#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QWidget>

class CreateAccountPage : public QWidget {
    Q_OBJECT
public:
    explicit CreateAccountPage(bool loginMode = false, QWidget *parent = nullptr);

signals:
    void backClicked();       // tells main.cpp "user hit back"
    void accountCreated();    // tells main.cpp "go to next screen"
};

#endif