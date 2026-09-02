#include "createaccount.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

CreateAccountPage::CreateAccountPage(QWidget *parent) : QWidget(parent) {
    QPushButton *backBtn = new QPushButton("< back");
    backBtn->setObjectName("backButton");

    QLabel *title = new QLabel("Create Account");
    title->setObjectName("caTitle");
    title->setAlignment(Qt::AlignCenter);

    QLabel *subtitle = new QLabel("Sign up and adopt you first cat today. ^^");
    subtitle->setObjectName("subtitle");
    subtitle->setAlignment(Qt::AlignCenter);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(backBtn);
    headerLayout->addStretch();

    QWidget *card = new QWidget();
    card->setObjectName("card");

    QLabel *idLabel = new QLabel("ID/Username");
    QLineEdit *idInput = new QLineEdit();
    idInput->setPlaceholderText("Enter your ID/Username");

    QLabel *pwLabel = new QLabel("Password");
    QLineEdit *pwInput = new QLineEdit();
    pwInput->setPlaceholderText("Enter Password");
    pwInput->setEchoMode(QLineEdit::Password);

    QCheckBox *agreeCheck = new QCheckBox("I agree to the Terms of Use and Privacy");

    QPushButton *createBtn = new QPushButton("Create Account");
    createBtn->setObjectName("primaryButton");

    QVBoxLayout *cardLayout = new QVBoxLayout();
    cardLayout->addWidget(idLabel);
    cardLayout->addWidget(idInput);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(pwLabel);
    cardLayout->addWidget(pwInput);
    cardLayout->addSpacing(6);
    cardLayout->addWidget(agreeCheck);
    cardLayout->addSpacing(14);
    cardLayout->addWidget(createBtn);
    cardLayout->setContentsMargins(30, 30, 30, 30);
    card->setLayout(cardLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(card);
    mainLayout->setContentsMargins(40, 20, 40, 40);
    setLayout(mainLayout);

    // wiring buttons to signals — main.cpp decides what happens
    connect(backBtn, &QPushButton::clicked, this, &CreateAccountPage::backClicked);
    connect(createBtn, &QPushButton::clicked, this, &CreateAccountPage::accountCreated);
}