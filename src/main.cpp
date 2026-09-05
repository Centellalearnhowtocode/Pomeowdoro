#include <QApplication>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QFrame>
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
    landingPage->setObjectName("landingPage");

    QLabel *logo = new QLabel("P");
    logo->setFixedSize(132, 132);
    logo->setObjectName("logo");
    logo->setAlignment(Qt::AlignCenter);

    QLabel *eyebrow = new QLabel("A GENTLER WAY TO FOCUS");
    eyebrow->setObjectName("landingEyebrow");
    eyebrow->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("Pomeowdoro");
    title->setObjectName("title");
    title->setAlignment(Qt::AlignCenter);

    QLabel *tagline = new QLabel(
        "Focus in small, cozy sessions and make room\nfor the things you want to finish.");
    tagline->setObjectName("landingTagline");
    tagline->setAlignment(Qt::AlignCenter);
    tagline->setWordWrap(true);

    QPushButton *getStartedBtn = new QPushButton("Start a focus session");
    getStartedBtn->setObjectName("landingPrimaryButton");

    QPushButton *loginBtn = new QPushButton("I already have an account");
    loginBtn->setObjectName("landingSecondaryButton");

    QLabel *status = new QLabel("25 min focus  |  5 min rest  |  4 sessions");
    status->setObjectName("landingStatus");
    status->setAlignment(Qt::AlignCenter);

    QVBoxLayout *landingLayout = new QVBoxLayout();
    landingLayout->addWidget(logo, 0, Qt::AlignCenter);
    landingLayout->addSpacing(12);
    landingLayout->addWidget(eyebrow);
    landingLayout->addSpacing(4);
    landingLayout->addWidget(title, 0, Qt::AlignCenter);
    landingLayout->addSpacing(8);
    landingLayout->addWidget(tagline);
    landingLayout->addSpacing(24);
    landingLayout->addWidget(getStartedBtn);
    landingLayout->addWidget(loginBtn);
    landingLayout->addSpacing(12);
    landingLayout->addWidget(status);
    landingLayout->setContentsMargins(120, 26, 120, 24);
    landingLayout->setSpacing(10);
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