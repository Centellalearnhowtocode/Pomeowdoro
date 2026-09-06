#ifndef NOTESPAGE_H
#define NOTESPAGE_H

#include <QSqlDatabase>
#include <QWidget>

class QLineEdit;
class QListWidget;
class QTextEdit;
class QLabel;

class NotesPage : public QWidget {
    Q_OBJECT

public:
    explicit NotesPage(QWidget *parent = nullptr);
    ~NotesPage() override;
    void setUser(const QString &username);

signals:
    void backClicked();

private slots:
    void saveNote();
    void updateNote();
    void deleteNote();
    void loadSelectedNote();
    void clearEditor();

private:
    void loadNotes();
    bool validateEditor();

    QSqlDatabase database;
    QListWidget *notesList;
    QLineEdit *titleEdit;
    QTextEdit *bodyEdit;
    QLabel *statusLabel;
    QLabel *emptyStateLabel;
    QString username;
    int selectedNoteId = -1;
};

#endif // NOTESPAGE_H