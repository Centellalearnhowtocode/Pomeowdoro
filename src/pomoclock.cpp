#include "pomoclock.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

PomoClock::PomoClock(QWidget *parent) : QWidget(parent) {
    QLabel *title = new QLabel("Pomeowdoro Clock");
    title->setObjectName("clockTitle");
    title->setAlignment(Qt::AlignCenter);

    timeLabel = new QLabel("25:00");
    timeLabel->setObjectName("timeLabel");
    timeLabel->setAlignment(Qt::AlignCenter);

    statusLabel = new QLabel("Loops completed: 0 | Remaining: 4");
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);

    totalTimeLabel = new QLabel("Total time today: 00:00:00");
    totalTimeLabel->setObjectName("totalTimeLabel");
    totalTimeLabel->setAlignment(Qt::AlignCenter);

    startBtn = new QPushButton("Start");
    startBtn->setObjectName("primaryButton");
    resetBtn = new QPushButton("Reset");
    resetBtn->setObjectName("secondaryButton");
    skipBtn = new QPushButton("Skip");
    skipBtn->setObjectName("secondaryButton");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(startBtn);
    buttonLayout->addWidget(resetBtn);
    buttonLayout->addWidget(skipBtn);

    workSpin = new QSpinBox();
    workSpin->setRange(1, 180);
    workSpin->setValue(25);
    workSpin->setSuffix(" min");

    breakSpin = new QSpinBox();
    breakSpin->setRange(1, 60);
    breakSpin->setValue(5);
    breakSpin->setSuffix(" min");

    sessionSpin = new QSpinBox();
    sessionSpin->setRange(1, 20);
    sessionSpin->setValue(4);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Work (min)", workSpin);
    formLayout->addRow("Break (min)", breakSpin);
    formLayout->addRow("Session (loops)", sessionSpin);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(title);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(totalTimeLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(formLayout);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(12);
    setLayout(mainLayout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PomoClock::tick);

    connect(startBtn, &QPushButton::clicked, this, &PomoClock::onStartClicked);
    connect(resetBtn, &QPushButton::clicked, this, &PomoClock::onResetClicked);
    connect(skipBtn, &QPushButton::clicked, this, &PomoClock::onSkipClicked);

    secondsRemaining = workSpin->value() * 60;
    updateTimeLabel();
    updateStatusLabel();
}

void PomoClock::startPhase(Phase phase) {
    currentPhase = phase;
    if (phase == Phase::Work) {
        secondsRemaining = workSpin->value() * 60;
    } else {
        secondsRemaining = breakSpin->value() * 60;
    }
    updateTimeLabel();
    updateStatusLabel();
}

void PomoClock::onStartClicked() {
    if (running) {
        // pause
        timer->stop();
        running = false;
        startBtn->setText("Start");
    } else {
        // start/resume
        timer->start(1000);
        running = true;
        startBtn->setText("Pause");
    }
}

void PomoClock::onResetClicked() {
    timer->stop();
    running = false;
    startBtn->setText("Start");
    loopsCompleted = 0;
    totalSecondsToday = 0;
    currentPhase = Phase::Work;
    secondsRemaining = workSpin->value() * 60;
    updateTimeLabel();
    updateStatusLabel();
    totalTimeLabel->setText("Total time today: 00:00:00");
}

void PomoClock::onSkipClicked() {
    if (currentPhase == Phase::Work) {
        loopsCompleted++;
        if (loopsCompleted >= sessionSpin->value()) {
            loopsCompleted = 0;
        }
        startPhase(Phase::Break);
    } else {
        startPhase(Phase::Work);
    }
}

void PomoClock::tick() {
    if (secondsRemaining > 0) {
        secondsRemaining--;
        if (currentPhase == Phase::Work) {
            totalSecondsToday++;
        }
        updateTimeLabel();

        int h = totalSecondsToday / 3600;
        int m = (totalSecondsToday % 3600) / 60;
        int s = totalSecondsToday % 60;
        totalTimeLabel->setText(QString("Total time today: %1:%2:%3")
            .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0')));
    } else {
        // phase finished, auto-advance
        if (currentPhase == Phase::Work) {
            loopsCompleted++;
            if (loopsCompleted >= sessionSpin->value()) {
                loopsCompleted = 0;
            }
            startPhase(Phase::Break);
        } else {
            startPhase(Phase::Work);
        }
    }
}

void PomoClock::updateTimeLabel() {
    int m = secondsRemaining / 60;
    int s = secondsRemaining % 60;
    timeLabel->setText(QString("%1:%2")
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0')));
}

void PomoClock::updateStatusLabel() {
    statusLabel->setText(QString("Loops completed: %1 | Remaining: %2")
        .arg(loopsCompleted)
        .arg(sessionSpin->value() - loopsCompleted));
}