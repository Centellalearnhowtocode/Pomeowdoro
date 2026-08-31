#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSqlDatabase>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Pomeowdoro - Qt6");
    window.resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *label = new QLabel("Welcome to Pomeowdoro!", &window);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QPushButton *btn = new QPushButton("Click Me", &window);
    layout->addWidget(btn);

    QObject::connect(btn, &QPushButton::clicked, [&]() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        label->setText("Qt6 Widgets + SQL module loaded!");
    });

    window.show();
    return app.exec();
}