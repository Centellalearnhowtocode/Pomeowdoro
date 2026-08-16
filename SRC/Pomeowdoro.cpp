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
      sessionCount(0)
{
    setWindowTitle("Pomodoro Clock");
    resize(360, 420);

    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(18);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    modeLabel = new QLabel("WORK", central);
    modeLabel->setAlignment(Qt::AlignCenter);
    modeLabel->setObjectName("modeLabel");

    timeLabel = new QLabel("25:00", central);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setObjectName("timeLabel");

    sessionLabel = new QLabel("Sessions completed: 0", central);
    sessionLabel->setAlignment(Qt::AlignCenter);
    sessionLabel->setObjectName("sessionLabel");

    auto *buttonRow = new QHBoxLayout();
    startPauseButton = new QPushButton("Start", central);
    resetButton = new QPushButton("Reset", central);
    buttonRow->addWidget(startPauseButton);
    buttonRow->addWidget(resetButton);

    auto *durationForm = new QFormLayout();
    workMinutesSpin = new QSpinBox(central);
    workMinutesSpin->setRange(1, 120);
    workMinutesSpin->setValue(25);
    breakMinutesSpin = new QSpinBox(central);
    breakMinutesSpin->setRange(1, 60);
    breakMinutesSpin->setValue(5);
    durationForm->addRow("Work (min):", workMinutesSpin);
    durationForm->addRow("Break (min):", breakMinutesSpin);

    mainLayout->addWidget(modeLabel);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(sessionLabel);
    mainLayout->addLayout(buttonRow);
    mainLayout->addLayout(durationForm);

    setCentralWidget(central);

    connect(startPauseButton, &QPushButton::clicked, this, &MainWindow::onStartPauseClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTick);
    connect(workMinutesSpin, &QSpinBox::valueChanged, this, &MainWindow::onDurationChanged);
    connect(breakMinutesSpin, &QSpinBox::valueChanged, this, &MainWindow::onDurationChanged);

    timer->setInterval(1000);

    applyStyle();
    updateDisplay();
}

void MainWindow::onStartPauseClicked() {
    isRunning = !isRunning;
    if (isRunning) {
        timer->start();
        startPauseButton->setText("Pause");
        workMinutesSpin->setEnabled(false);
        breakMinutesSpin->setEnabled(false);
    } else {
        timer->stop();
        startPauseButton->setText("Start");
    }
}

void MainWindow::onResetClicked() {
    timer->stop();
    isRunning = false;
    startPauseButton->setText("Start");
    workMinutesSpin->setEnabled(true);
    breakMinutesSpin->setEnabled(true);
    currentMode = Mode::Work;
    secondsRemaining = workMinutesSpin->value() * 60;
    sessionCount = 0;
    updateDisplay();
}

void MainWindow::onTick() {
    if (secondsRemaining > 0) {
        secondsRemaining--;
        updateDisplay();
    } else {
        if (currentMode == Mode::Work) {
            sessionCount++;
            switchMode(Mode::Break);
        } else {
            switchMode(Mode::Work);
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
    modeLabel->setText(currentMode == Mode::Work ? "WORK" : "BREAK");
    sessionLabel->setText(QString("Sessions completed: %1").arg(sessionCount));
}

void MainWindow::applyStyle() {
    // Colors/fonts kept simple and centralized here so they're easy to
    // swap out later when the theme/font picker is built.
    setStyleSheet(R"(
        QWidget {
            background-color: #2b0f14;
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
        QPushButton {
            background-color: #7a2b3c;
            border: none;
            border-radius: 6px;
            padding: 10px 18px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #93394d;
        }
        QSpinBox {
            background-color: #3d161d;
            border: 1px solid #7a2b3c;
            border-radius: 4px;
            padding: 4px;
        }
    )");
}