#include "Pomeowdoro.h"

#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      timer(new QTimer(this)),
      secondsRemaining(25 * 60),
      isRunning(false),
      currentMode(Mode::Work),
      sessionCount(0),
      totalElapsedSeconds(0),
      loopsRemaining(4)
{
    setWindowTitle("Pomeowdoro Clock");
    resize(600, 320);

    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(18);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    modeLabel = new QLabel("Pomeowdoro", central);
    modeLabel->setAlignment(Qt::AlignCenter);
    modeLabel->setObjectName("modeLabel");

    timeLabel = new QLabel("25:00", central);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setObjectName("timeLabel");

    sessionLabel = new QLabel("Loops completed: 0", central);
    sessionLabel->setAlignment(Qt::AlignCenter);
    sessionLabel->setObjectName("sessionLabel");

    totalTimeLabel = new QLabel("Total time today: 00:00:00", central);
    totalTimeLabel->setAlignment(Qt::AlignCenter);
    totalTimeLabel->setObjectName("totalTimeLabel");

    auto *buttonRow = new QHBoxLayout();
    startPauseButton = new QPushButton("Start", central);
    resetButton = new QPushButton("Reset", central);
    skipButton = new QPushButton("Skip", central);
    buttonRow->addWidget(startPauseButton);
    buttonRow->addWidget(resetButton);
    buttonRow->addWidget(skipButton);

    auto *durationForm = new QFormLayout();
    workMinutesSpin = new QSpinBox(central);
    workMinutesSpin->setRange(1, 120);
    workMinutesSpin->setValue(25);
    breakMinutesSpin = new QSpinBox(central);
    breakMinutesSpin->setRange(1, 60);
    breakMinutesSpin->setValue(5);
    durationForm->addRow("Work (min):", workMinutesSpin);
    durationForm->addRow("Break (min):", breakMinutesSpin);

    sessionLoopsSpin = new QSpinBox(central);
    sessionLoopsSpin->setRange(1, 20);
    sessionLoopsSpin->setValue(4);
    durationForm->addRow("Session (loops):", sessionLoopsSpin);

    mainLayout->addWidget(modeLabel);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(sessionLabel);
    mainLayout->addWidget(totalTimeLabel);
    mainLayout->addLayout(buttonRow);
    mainLayout->addLayout(durationForm);

    setCentralWidget(central);

    connect(startPauseButton, &QPushButton::clicked, this, &MainWindow::onStartPauseClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(skipButton, &QPushButton::clicked, this, &MainWindow::onSkipClicked);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTick);
    connect(workMinutesSpin, &QSpinBox::valueChanged, this, &MainWindow::onDurationChanged);
    connect(breakMinutesSpin, &QSpinBox::valueChanged, this, &MainWindow::onDurationChanged);
    connect(sessionLoopsSpin, &QSpinBox::valueChanged, this, [this](int value) {
        if (!isRunning) {
            loopsRemaining = value;
            updateDisplay();
        }
    });

    timer->setInterval(1000);

    applyStyle();
    updateDisplay();
}

void MainWindow::onStartPauseClicked() {
    if (loopsRemaining <= 0) {
        return; // already done, must Reset first
    }
    isRunning = !isRunning;
    if (isRunning) {
        timer->start();
        startPauseButton->setText("Pause");
        workMinutesSpin->setEnabled(false);
        breakMinutesSpin->setEnabled(false);
        sessionLoopsSpin->setEnabled(false);
    } else {
        timer->stop();
        startPauseButton->setText("Start");
    }
}

void MainWindow::onResetClicked() {
    timer->stop();
    isRunning = false;
    startPauseButton->setText("Start");
    startPauseButton->setEnabled(true);
    workMinutesSpin->setEnabled(true);
    breakMinutesSpin->setEnabled(true);
    sessionLoopsSpin->setEnabled(true);
    currentMode = Mode::Work;
    secondsRemaining = workMinutesSpin->value() * 60;
    sessionCount = 0;
    totalElapsedSeconds = 0;
    loopsRemaining = sessionLoopsSpin->value();
    updateDisplay();
}

void MainWindow::completeLoop() {
    sessionCount++;
    if (loopsRemaining > 0) {
        loopsRemaining--;
    }
    if (loopsRemaining <= 0) {
        timer->stop();
        isRunning = false;
        startPauseButton->setText("Done");
        startPauseButton->setEnabled(false);
        updateDisplay();
        return;
    }
    switchMode(Mode::Work);
}

void MainWindow::onSkipClicked() {
    if (loopsRemaining <= 0) {
        return;
    }
    // 1 loop = 1 Work block + 1 Break block.
    // Loop count only decreases once Break finishes.
    if (currentMode == Mode::Work) {
        switchMode(Mode::Break);
    } else {
        completeLoop();
    }
}

void MainWindow::onTick() {
    totalElapsedSeconds++;

    if (secondsRemaining > 0) {
        secondsRemaining--;
        updateDisplay();
    } else {
        if (currentMode == Mode::Work) {
            switchMode(Mode::Break);
        } else {
            completeLoop();
        }
    }
}

void MainWindow::onDurationChanged() {
    if (!isRunning) {
        secondsRemaining = (currentMode == Mode::Work ? workMinutesSpin->value() : breakMinutesSpin->value()) * 60;
        updateDisplay();
    }
}

void MainWindow::switchMode(Mode newMode) {
    currentMode = newMode;
    secondsRemaining = (currentMode == Mode::Work ? workMinutesSpin->value() : breakMinutesSpin->value()) * 60;
    updateDisplay();
}

void MainWindow::updateDisplay() {
    int minutes = secondsRemaining / 60;
    int seconds = secondsRemaining % 60;
    timeLabel->setText(QString("%1:%2")
                            .arg(minutes, 2, 10, QChar('0'))
                            .arg(seconds, 2, 10, QChar('0')));
    modeLabel->setText(loopsRemaining <= 0 ? "DONE" : (currentMode == Mode::Work ? "Pomeowdoro Clock" : "BREAK"));
    sessionLabel->setText(QString("Loops completed: %1  |  Remaining: %2").arg(sessionCount).arg(loopsRemaining));

    int totalHours = totalElapsedSeconds / 3600;
    int totalMinutes = (totalElapsedSeconds % 3600) / 60;
    int totalSecs = totalElapsedSeconds % 60;
    totalTimeLabel->setText(QString("Total time today: %1:%2:%3")
                                 .arg(totalHours, 2, 10, QChar('0'))
                                 .arg(totalMinutes, 2, 10, QChar('0'))
                                 .arg(totalSecs, 2, 10, QChar('0')));
}

void MainWindow::applyStyle() {
  //QSS for UI
    setStyleSheet(R"(
        QWidget {
            background: #2b0f14;
            color: #f5e6e8;
            font-family: "Segoe UI";
        }
        QLabel#modeLabel {
            font-size: 18px;
            font-weight: bold;
            letter-spacing: 4px;
            color: #d98ea1;
        }
        QLabel#timeLabel {
            font-size: 56px;
            font-weight: bold;
        }
        QLabel#sessionLabel {
            font-size: 13px;
            color: #c9a0a8;
        }
        QLabel#totalTimeLabel {
            font-size: 12px;
            color: #a5828a;
        }
        QPushButton {
            background: #7a2b3c;
            border: none;
            border-radius: 6px;
            padding: 10px 18px;
            font-size: 14px;
        }
        QPushButton:hover {
            background: #93394d;
        }
        QPushButton:disabled {
            background: #4a1e28;
            color: #8a6a70;
        }
        QSpinBox {
            background: #3d161d;
            border: 1px solid #7a2b3c;
            border-radius: 4px;
            padding: 4px;
        }
    )");
}