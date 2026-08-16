#pragma once

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QSpinBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onStartPauseClicked();
    void onResetClicked();
    void onTick();
    void onDurationChanged();

private:
    enum class Mode { Work, Break };

    void updateDisplay();
    void switchMode(Mode newMode);
    void applyStyle();

    QTimer *timer;
    int secondsRemaining;
    bool isRunning;
    Mode currentMode;
    int sessionCount;

    QLabel *modeLabel;
    QLabel *timeLabel;
    QLabel *sessionLabel;
    QPushButton *startPauseButton;
    QPushButton *resetButton;
    QSpinBox *workMinutesSpin;
    QSpinBox *breakMinutesSpin;
};