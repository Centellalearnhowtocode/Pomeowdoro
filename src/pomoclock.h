#ifndef POMOCLOCK_H
#define POMOCLOCK_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QProgressBar>

class PomoClock : public QWidget {
    Q_OBJECT

public:
    explicit PomoClock(QWidget *parent = nullptr);

private slots:
    void onStartClicked();
    void onResetClicked();
    void onSkipClicked();
    void tick();

private:
    enum class Phase { Work, Break };

    void updateTimeLabel();
    void updateStatusLabel();
    void startPhase(Phase phase);

    QLabel *timeLabel;
    QLabel *phaseLabel;
    QLabel *statusLabel;
    QLabel *totalTimeLabel;
    QProgressBar *progressBar;

    QPushButton *startBtn;
    QPushButton *resetBtn;
    QPushButton *skipBtn;

    QSpinBox *workSpin;
    QSpinBox *breakSpin;
    QSpinBox *sessionSpin;

    QTimer *timer;
    Phase currentPhase = Phase::Work;
    int secondsRemaining = 0;
    int loopsCompleted = 0;
    int totalSecondsToday = 0;
    bool running = false;
};

#endif // POMOCLOCK_H