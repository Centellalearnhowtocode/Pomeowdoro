#include "createaccount.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QFrame>

CreateAccountPage::CreateAccountPage(QWidget *parent) : QWidget(parent) {
    QPushButton *backBtn = new QPushButton("<  Back");
    backBtn->setObjectName("backButton");

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(backBtn);
    headerLayout->addStretch();

    QWidget *welcomePanel = new QWidget();
    welcomePanel->setObjectName("welcomePanel");

    QLabel *catMark = new QLabel("P");
    catMark->setObjectName("catMark");
    catMark->setAlignment(Qt::AlignCenter);

    QLabel *welcomeTitle = new QLabel("Make time\nfor what matters.");
    welcomeTitle->setObjectName("welcomeTitle");

    QLabel *welcomeText = new QLabel(
        "Create your Pomeowdoro account\nand make every focus session feel\na little more playful.");
    welcomeText->setObjectName("welcomeText");
    welcomeText->setWordWrap(true);

    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomePanel);
    welcomeLayout->addWidget(catMark, 0, Qt::AlignLeft);
    welcomeLayout->addSpacing(24);
    welcomeLayout->addWidget(welcomeTitle);
    welcomeLayout->addSpacing(12);
    welcomeLayout->addWidget(welcomeText);
    welcomeLayout->addStretch();
    welcomeLayout->setContentsMargins(30, 30, 28, 30);

    QWidget *formPanel = new QWidget();
    formPanel->setObjectName("formPanel");

    QLabel *eyebrow = new QLabel("START YOUR FOCUS JOURNEY");
    eyebrow->setObjectName("eyebrow");

    QLabel *title = new QLabel("Create account");
    title->setObjectName("caTitle");

    QLabel *subtitle = new QLabel("Your cozy workspace is one step away.");
    subtitle->setObjectName("subtitle");
    subtitle->setWordWrap(true);

    QLabel *idLabel = new QLabel("ID/Username");
    idLabel->setObjectName("fieldLabel");
    QLineEdit *idInput = new QLineEdit();
    idInput->setObjectName("formInput");
    idInput->setPlaceholderText("Enter your ID/Username");

    QLabel *pwLabel = new QLabel("Password");
    pwLabel->setObjectName("fieldLabel");
    QLineEdit *pwInput = new QLineEdit();
    pwInput->setObjectName("formInput");
    pwInput->setPlaceholderText("Enter Password");
    pwInput->setEchoMode(QLineEdit::Password);

    QCheckBox *agreeCheck = new QCheckBox("I agree to the Terms of Use and Privacy");
    agreeCheck->setObjectName("termsCheck");

    QPushButton *createBtn = new QPushButton("Create Account");
    createBtn->setObjectName("primaryButton");

    QVBoxLayout *formLayout = new QVBoxLayout(formPanel);
    formLayout->addWidget(eyebrow);
    formLayout->addSpacing(7);
    formLayout->addWidget(title);
    formLayout->addWidget(subtitle);
    formLayout->addSpacing(22);
    formLayout->addWidget(idLabel);
    formLayout->addWidget(idInput);
    formLayout->addSpacing(13);
    formLayout->addWidget(pwLabel);
    formLayout->addWidget(pwInput);
    formLayout->addSpacing(10);
    formLayout->addWidget(agreeCheck);
    formLayout->addSpacing(16);
    formLayout->addWidget(createBtn);
    formLayout->setContentsMargins(32, 28, 32, 28);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(headerLayout);
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->addWidget(welcomePanel, 2);
    contentLayout->addWidget(formPanel, 3);
    contentLayout->setSpacing(18);
    mainLayout->addLayout(contentLayout);
    mainLayout->setContentsMargins(28, 18, 28, 28);
    mainLayout->setSpacing(14);
    setLayout(mainLayout);

    // wiring buttons to signals — main.cpp decides what happens
    connect(backBtn, &QPushButton::clicked, this, &CreateAccountPage::backClicked);
    connect(createBtn, &QPushButton::clicked, this, &CreateAccountPage::accountCreated);
}