#include "notespage.h"

#include <QDateTime>
#include <QDir>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSqlQuery>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QStandardPaths>

NotesPage::NotesPage(QWidget *parent) : QWidget(parent) {
    setObjectName("notesPage");

    const QString databasePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(databasePath);
    database = QSqlDatabase::addDatabase("QSQLITE", "pomeowdoro_notes");
    database.setDatabaseName(databasePath + "/notes.db");
    database.open();

    QSqlQuery createQuery(database);
    createQuery.exec("CREATE TABLE IF NOT EXISTS notes (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT NOT NULL, body TEXT NOT NULL, updated_at TEXT NOT NULL)");

    auto *backButton = new QPushButton("<  BACK TO CLOCK");
    backButton->setObjectName("backButton");
    connect(backButton, &QPushButton::clicked, this, &NotesPage::backClicked);

    auto *title = new QLabel("Notes");
    title->setObjectName("notesTitle");
    auto *subtitle = new QLabel("Keep ideas, reminders, and small wins close by.");
    subtitle->setObjectName("notesSubtitle");

    notesList = new QListWidget();
    notesList->setObjectName("notesList");
    connect(notesList, &QListWidget::itemClicked, this, [this](QListWidgetItem *) {
        loadSelectedNote();
    });

    titleEdit = new QLineEdit();
    titleEdit->setObjectName("notesInput");
    titleEdit->setPlaceholderText("Note title");

    bodyEdit = new QTextEdit();
    bodyEdit->setObjectName("notesEditor");
    bodyEdit->setPlaceholderText("Write your note here...");

    auto *newButton = new QPushButton("New note");
    newButton->setObjectName("notesSecondaryButton");
    auto *saveButton = new QPushButton("Save note");
    saveButton->setObjectName("notesPrimaryButton");
    auto *updateButton = new QPushButton("Update");
    updateButton->setObjectName("notesSecondaryButton");
    auto *deleteButton = new QPushButton("Delete");
    deleteButton->setObjectName("notesDangerButton");

    connect(newButton, &QPushButton::clicked, this, &NotesPage::clearEditor);
    connect(saveButton, &QPushButton::clicked, this, &NotesPage::saveNote);
    connect(updateButton, &QPushButton::clicked, this, &NotesPage::updateNote);
    connect(deleteButton, &QPushButton::clicked, this, &NotesPage::deleteNote);

    statusLabel = new QLabel();
    statusLabel->setObjectName("notesStatus");

    auto *editorForm = new QFormLayout();
    editorForm->addRow("Title", titleEdit);
    editorForm->addRow("Note", bodyEdit);
    editorForm->setLabelAlignment(Qt::AlignTop);
    editorForm->setSpacing(10);

    auto *actions = new QHBoxLayout();
    actions->addWidget(newButton);
    actions->addStretch();
    actions->addWidget(deleteButton);
    actions->addWidget(updateButton);
    actions->addWidget(saveButton);

    auto *editorPanel = new QWidget();
    editorPanel->setObjectName("notesEditorPanel");
    auto *editorLayout = new QVBoxLayout(editorPanel);
    editorLayout->addLayout(editorForm);
    editorLayout->addLayout(actions);
    editorLayout->addWidget(statusLabel);

    auto *listPanel = new QWidget();
    listPanel->setObjectName("notesListPanel");
    auto *listLayout = new QVBoxLayout(listPanel);
    auto *listTitle = new QLabel("Your notes");
    listTitle->setObjectName("notesSectionTitle");
    emptyStateLabel = new QLabel("No notes yet\nCreate one to keep your\nthoughts close by.");
    emptyStateLabel->setObjectName("notesEmptyState");
    emptyStateLabel->setAlignment(Qt::AlignCenter);
    emptyStateLabel->setWordWrap(true);
    listLayout->addWidget(listTitle);
    listLayout->addWidget(notesList);
    listLayout->addWidget(emptyStateLabel);

    auto *content = new QHBoxLayout();
    content->addWidget(listPanel, 1);
    content->addWidget(editorPanel, 2);
    content->setSpacing(18);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(backButton, 0, Qt::AlignLeft);
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addLayout(content, 1);
    layout->setContentsMargins(34, 24, 34, 26);
    layout->setSpacing(8);

    loadNotes();
}

NotesPage::~NotesPage() {
    if (database.isOpen()) database.close();
    const QString connectionName = database.connectionName();
    database = QSqlDatabase();
    QSqlDatabase::removeDatabase(connectionName);
}

void NotesPage::loadNotes() {
    notesList->clear();
    QSqlQuery query(database);
    query.exec("SELECT id, title, body FROM notes ORDER BY updated_at DESC");
    while (query.next()) {
        auto *item = new QListWidgetItem(query.value(1).toString());
        item->setData(Qt::UserRole, query.value(0));
        item->setToolTip(query.value(2).toString());
        notesList->addItem(item);
    }
    const bool hasNotes = notesList->count() > 0;
    emptyStateLabel->setVisible(!hasNotes);
    notesList->setVisible(hasNotes);
}

bool NotesPage::validateEditor() {
    if (titleEdit->text().trimmed().isEmpty() && bodyEdit->toPlainText().trimmed().isEmpty()) {
        statusLabel->setText("Add a title or some text before saving.");
        return false;
    }
    return true;
}

void NotesPage::saveNote() {
    if (!validateEditor()) return;
    QSqlQuery query(database);
    query.prepare("INSERT INTO notes (title, body, updated_at) VALUES (?, ?, ?)");
    query.addBindValue(titleEdit->text().trimmed().isEmpty() ? "Untitled note" : titleEdit->text().trimmed());
    query.addBindValue(bodyEdit->toPlainText());
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    if (query.exec()) {
        loadNotes();
        clearEditor();
        statusLabel->setText("Note saved.");
    } else {
        statusLabel->setText("Could not save this note.");
    }
}

void NotesPage::updateNote() {
    if (selectedNoteId < 0 || !validateEditor()) return;
    QSqlQuery query(database);
    query.prepare("UPDATE notes SET title = ?, body = ?, updated_at = ? WHERE id = ?");
    query.addBindValue(titleEdit->text().trimmed().isEmpty() ? "Untitled note" : titleEdit->text().trimmed());
    query.addBindValue(bodyEdit->toPlainText());
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    query.addBindValue(selectedNoteId);
    if (query.exec()) {
        loadNotes();
        statusLabel->setText("Note updated.");
    } else {
        statusLabel->setText("Could not update this note.");
    }
}

void NotesPage::deleteNote() {
    if (selectedNoteId < 0) {
        statusLabel->setText("Select a note to delete.");
        return;
    }
    QSqlQuery query(database);
    query.prepare("DELETE FROM notes WHERE id = ?");
    query.addBindValue(selectedNoteId);
    if (query.exec()) {
        loadNotes();
        clearEditor();
        statusLabel->setText("Note deleted.");
    } else {
        statusLabel->setText("Could not delete this note.");
    }
}

void NotesPage::loadSelectedNote() {
    auto *item = notesList->currentItem();
    if (!item) return;
    selectedNoteId = item->data(Qt::UserRole).toInt();
    QSqlQuery query(database);
    query.prepare("SELECT title, body FROM notes WHERE id = ?");
    query.addBindValue(selectedNoteId);
    if (query.exec() && query.next()) {
        titleEdit->setText(query.value(0).toString());
        bodyEdit->setPlainText(query.value(1).toString());
        statusLabel->setText("Editing selected note.");
    }
}

void NotesPage::clearEditor() {
    selectedNoteId = -1;
    notesList->clearSelection();
    titleEdit->clear();
    bodyEdit->clear();
    statusLabel->setText("Ready for a new note.");
    titleEdit->setFocus();
}