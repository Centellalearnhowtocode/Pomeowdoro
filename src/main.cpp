#include <QApplication>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include "createaccount.h"
#include "pomoclock.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(QLatin1String(styleFile.readAll()));
    }

    QStackedWidget *stack = new QStackedWidget();
    stack->setWindowTitle("Pomeowdoro");
    stack->resize(788, 456);

    // ---------- PAGE 0: Landing ----------
    QWidget *landingPage = new QWidget();

    QLabel *logo = new QLabel();
    logo->setFixedSize(180, 180);
    logo->setObjectName("logo");
    logo->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("Pomeowdoro APP");
    title->setObjectName("title");
    title->setAlignment(Qt::AlignCenter);

    QPushButton *getStartedBtn = new QPushButton("Get start for free");
    getStartedBtn->setObjectName("primaryButton");

    QPushButton *loginBtn = new QPushButton("Already have an account?");
    loginBtn->setObjectName("primaryButton");

    QVBoxLayout *landingLayout = new QVBoxLayout();
    landingLayout->addStretch();
    landingLayout->addWidget(logo, 0, Qt::AlignCenter);
    landingLayout->addWidget(title, 0, Qt::AlignCenter);
    landingLayout->addSpacing(30);
    landingLayout->addWidget(getStartedBtn);
    landingLayout->addWidget(loginBtn);
    landingLayout->addStretch();
    landingLayout->setContentsMargins(100, 40, 100, 40);
    landingLayout->setSpacing(16);
    landingPage->setLayout(landingLayout);

    // ---------- PAGE 1: Create Account ----------
    CreateAccountPage *createAccountPage = new CreateAccountPage();

    // ---------- PAGE 2: Clock ----------
    PomoClock *clockPage = new PomoClock();

    // ---------- Add pages to stack ----------
    stack->addWidget(landingPage);        // index 0
    stack->addWidget(createAccountPage);  // index 1
    stack->addWidget(clockPage);          // index 2

    // ---------- Wiring ----------
    QObject::connect(getStartedBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(clockPage);
    });

    QObject::connect(loginBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(createAccountPage);
    });

    QObject::connect(createAccountPage, &CreateAccountPage::backClicked, [=]() {
        stack->setCurrentWidget(landingPage);
    });

    QObject::connect(createAccountPage, &CreateAccountPage::accountCreated, [=]() {
        stack->setCurrentWidget(clockPage);
    });

    stack->show();

    return app.exec();
}