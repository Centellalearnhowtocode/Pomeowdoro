#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QWidget>

class CreateAccountPage : public QWidget {
    Q_OBJECT
public:
    explicit CreateAccountPage(bool loginMode = false, QWidget *parent = nullptr);

signals:
    void backClicked();
    void accountCreated(const QString &username);
    void authenticationFailed(const QString &message);
};

#endif