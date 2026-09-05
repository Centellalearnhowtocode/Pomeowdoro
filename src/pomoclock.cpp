#include "pomoclock.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>

PomoClock::PomoClock(QWidget *parent) : QWidget(parent) {
    workSpin = new QSpinBox();
    workSpin->setObjectName("settingSpin");
    workSpin->setRange(1, 180);
    workSpin->setValue(25);

    breakSpin = new QSpinBox();
    breakSpin->setObjectName("settingSpin");
    breakSpin->setRange(1, 60);
    breakSpin->setValue(5);

    sessionSpin = new QSpinBox();
    sessionSpin->setObjectName("settingSpin");
    sessionSpin->setRange(1, 20);
    sessionSpin->setValue(3);

    QLabel *title = new QLabel("Pomeowdoro Clock");
    title->setObjectName("clockTitle");
    title->setAlignment(Qt::AlignCenter);

    phaseLabel = new QLabel("FOCUS SESSION");
    phaseLabel->setObjectName("phaseLabel");
    phaseLabel->setAlignment(Qt::AlignCenter);

    timeLabel = new QLabel("25:00");
    timeLabel->setObjectName("timeLabel");
    timeLabel->setAlignment(Qt::AlignCenter);

    statusLabel = new QLabel("Session 1 of 3");
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);

    progressBar = new QProgressBar();
    progressBar->setObjectName("timerProgress");
    progressBar->setRange(0, 1500);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);

    totalTimeLabel = new QLabel("Total time today: 00:00:00");
    totalTimeLabel->setObjectName("totalTimeLabel");
    totalTimeLabel->setAlignment(Qt::AlignCenter);

    startBtn = new QPushButton("> ");
    startBtn->setObjectName("clockPrimaryButton");
    resetBtn = new QPushButton("↻");
    resetBtn->setObjectName("clockSecondaryButton");
    skipBtn = new QPushButton("Skip");
    skipBtn->setObjectName("clockSecondaryButton");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(18);
    buttonLayout->addWidget(startBtn);
    buttonLayout->addWidget(resetBtn);

    auto makeSettingCard = [](const QString &label, QSpinBox *spin) {
        QWidget *card = new QWidget();
        card->setObjectName("settingCard");
        QLabel *cardLabel = new QLabel(label);
        cardLabel->setObjectName("settingLabel");
        cardLabel->setAlignment(Qt::AlignCenter);
        spin->setAlignment(Qt::AlignCenter);
        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->addWidget(cardLabel);
        layout->addWidget(spin);
        layout->setContentsMargins(12, 14, 12, 12);
        layout->setSpacing(4);
        return card;
    };

    QWidget *timerArea = new QWidget();
    timerArea->setObjectName("timerArea");
    QVBoxLayout *timerLayout = new QVBoxLayout(timerArea);
    timerLayout->addWidget(phaseLabel);
    timerLayout->addWidget(timeLabel);
    timerLayout->addWidget(progressBar);
    timerLayout->addWidget(totalTimeLabel);
    timerLayout->addSpacing(8);
    timerLayout->addLayout(buttonLayout);
    timerLayout->setContentsMargins(0, 0, 0, 0);
    timerLayout->setSpacing(6);

    QLabel *settingsIcon = new QLabel("SETTINGS");
    settingsIcon->setObjectName("railIcon");
    QLabel *notesIcon = new QLabel("NOTES");
    notesIcon->setObjectName("railIcon");
    QLabel *statsIcon = new QLabel("STATS");
    statsIcon->setObjectName("railIcon");

    QVBoxLayout *railLayout = new QVBoxLayout();
    railLayout->addWidget(settingsIcon);
    railLayout->addWidget(notesIcon);
    railLayout->addWidget(statsIcon);
    railLayout->addWidget(skipBtn);
    railLayout->addStretch();
    railLayout->setSpacing(14);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(timerArea, 1);
    topLayout->addLayout(railLayout);
    topLayout->addStretch();
    topLayout->setSpacing(28);

    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(makeSettingCard("Session length", workSpin));
    settingsLayout->addWidget(makeSettingCard("Break length", breakSpin));
    settingsLayout->addWidget(makeSettingCard("Sessions", sessionSpin));
    settingsLayout->setSpacing(18);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(title);
    mainLayout->addSpacing(2);
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addLayout(settingsLayout);
    mainLayout->setContentsMargins(34, 26, 34, 26);
    mainLayout->setSpacing(8);
    setLayout(mainLayout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PomoClock::tick);

    connect(startBtn, &QPushButton::clicked, this, &PomoClock::onStartClicked);
    connect(resetBtn, &QPushButton::clicked, this, &PomoClock::onResetClicked);
    connect(skipBtn, &QPushButton::clicked, this, &PomoClock::onSkipClicked);

    secondsRemaining = workSpin->value() * 60;
    updateTimeLabel();
    updateStatusLabel();
    startPhase(Phase::Work);
}

void PomoClock::startPhase(Phase phase) {
    currentPhase = phase;
    if (phase == Phase::Work) {
        secondsRemaining = workSpin->value() * 60;
    } else {
        secondsRemaining = breakSpin->value() * 60;
    }
    phaseLabel->setText(phase == Phase::Work ? "FOCUS SESSION" : "REST SESSION");
    progressBar->setRange(0, secondsRemaining);
    progressBar->setValue(0);
    updateTimeLabel();
    updateStatusLabel();
}

void PomoClock::onStartClicked() {
    if (running) {
        // pause
        timer->stop();
        running = false;
        startBtn->setText("> ");
    } else {
        // start/resume
        timer->start(1000);
        running = true;
        startBtn->setText("||");
    }
}

void PomoClock::onResetClicked() {
    timer->stop();
    running = false;
    startBtn->setText("> ");
    loopsCompleted = 0;
    totalSecondsToday = 0;
    startPhase(Phase::Work);
    totalTimeLabel->setText("Focused today  00:00:00");
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
        progressBar->setValue(progressBar->maximum() - secondsRemaining);

        int h = totalSecondsToday / 3600;
        int m = (totalSecondsToday % 3600) / 60;
        int s = totalSecondsToday % 60;
        totalTimeLabel->setText(QString("Focused today  %1:%2:%3")
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
    statusLabel->setText(QString("Session %1 of %2")
        .arg(loopsCompleted + 1)
        .arg(sessionSpin->value()));
}