#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QFont>

// ---------------------------------------------------------
// CatIconWidget — draws the pink circle + hand-drawn cat face
// ---------------------------------------------------------
class CatIconWidget : public QWidget {
public:
    explicit CatIconWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(300, 300);
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        const QColor circleFill("#FBE0E6");
        const QColor lineColor("#7A2340");
        const QRectF bounds(0, 0, width(), height());

        // Soft drop shadow behind the circle
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, 60));
        p.drawEllipse(bounds.adjusted(10, 14, 4, 20));

        // Pink circle background
        p.setBrush(circleFill);
        p.drawEllipse(bounds.adjusted(4, 4, -4, -4));

        // Cat face line art
        QPen pen(lineColor);
        pen.setWidth(4);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);

        const QPointF c = bounds.center();

        // Face outline (rounded square-ish head)
        QPainterPath face;
        face.addRoundedRect(QRectF(c.x() - 85, c.y() - 55, 170, 130), 45, 45);
        p.drawPath(face);

        // Ears
        QPainterPath earL, earR;
        earL.moveTo(c.x() - 70, c.y() - 55);
        earL.lineTo(c.x() - 95, c.y() - 115);
        earL.lineTo(c.x() - 30, c.y() - 75);
        earR.moveTo(c.x() + 70, c.y() - 55);
        earR.lineTo(c.x() + 95, c.y() - 115);
        earR.lineTo(c.x() + 30, c.y() - 75);
        p.drawPath(earL);
        p.drawPath(earR);

        // Forehead tuft lines
        for (int i = -1; i <= 1; ++i) {
            p.drawLine(QPointF(c.x() + i * 14, c.y() - 55),
                       QPointF(c.x() + i * 14, c.y() - 80));
        }

        // Eyes (filled dots)
        p.setBrush(lineColor);
        p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(c.x() - 32, c.y() - 5), 7, 7);
        p.drawEllipse(QPointF(c.x() + 32, c.y() - 5), 7, 7);

        // Nose/mouth (small x)
        pen.setWidth(3);
        p.setPen(pen);
        p.drawLine(QPointF(c.x() - 8, c.y() + 25), QPointF(c.x() + 8, c.y() + 41));
        p.drawLine(QPointF(c.x() + 8, c.y() + 25), QPointF(c.x() - 8, c.y() + 41));

        // Whiskers
        p.drawLine(QPointF(c.x() - 85, c.y() + 5), QPointF(c.x() - 130, c.y() - 3));
        p.drawLine(QPointF(c.x() - 85, c.y() + 15), QPointF(c.x() - 130, c.y() + 15));
        p.drawLine(QPointF(c.x() + 85, c.y() + 5), QPointF(c.x() + 130, c.y() - 3));
        p.drawLine(QPointF(c.x() + 85, c.y() + 15), QPointF(c.x() + 130, c.y() + 15));
    }
};

// ---------------------------------------------------------
// NeonTitleWidget — layered chromatic-outline title text
// ---------------------------------------------------------
class NeonTitleWidget : public QWidget {
public:
    explicit NeonTitleWidget(const QString &text, QWidget *parent = nullptr)
        : QWidget(parent), m_text(text) {
        setFixedHeight(90);
        setMinimumWidth(560);
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        QFont f("Arial", 40, QFont::Black);
        f.setLetterSpacing(QFont::AbsoluteSpacing, 1);
        p.setFont(f);

        QRect r = rect();

        // Cyan duplicate, offset down-right
        p.setPen(QColor("#26D9C9"));
        p.drawText(r.translated(4, 4), Qt::AlignLeft | Qt::AlignVCenter, m_text);

        // Magenta duplicate, offset up-left
        p.setPen(QColor("#E040FB"));
        p.drawText(r.translated(-4, -4), Qt::AlignLeft | Qt::AlignVCenter, m_text);

        // Solid gray main text on top
        p.setPen(QColor("#5C5C5C"));
        p.drawText(r, Qt::AlignLeft | Qt::AlignVCenter, m_text);
    }

private:
    QString m_text;
};

// ---------------------------------------------------------
// PillButton — rounded button with a hard offset shadow
// (built as a small container: shadow frame + button on top)
// ---------------------------------------------------------
QWidget *makePillButton(const QString &text, QPushButton *&outButton) {
    QWidget *container = new QWidget;
    container->setFixedHeight(78);
    container->setMinimumWidth(500);

    // Hard "shadow" frame peeking out bottom-right
    QLabel *shadow = new QLabel(container);
    shadow->setStyleSheet("background-color: #C23158; border-radius: 28px;");

    QPushButton *button = new QPushButton(text, container);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #FBE0E6;"
        "  color: #595959;"
        "  border-radius: 28px;"
        "  font-size: 17px;"
        "  font-weight: 700;"
        "}"
        "QPushButton:hover { background-color: #FDEAEF; }"
        "QPushButton:pressed { background-color: #F4CBD3; }"
    );

    // Position shadow slightly behind/below the button
    QObject::connect(container, &QWidget::destroyed, [](QObject*){});
    auto layoutChildren = [container, shadow, button]() {
        QRect r = container->rect();
        shadow->setGeometry(r.adjusted(0, 6, -6, 0));
        button->setGeometry(r.adjusted(0, 0, -6, -6));
    };
    layoutChildren();

    // Re-layout on resize
    class ResizeHelper : public QObject {
    public:
        ResizeHelper(QWidget *c, QLabel *s, QPushButton *b) : container(c), shadow(s), button(b) {}
        bool eventFilter(QObject *, QEvent *event) override {
            if (event->type() == QEvent::Resize) {
                QRect r = container->rect();
                shadow->setGeometry(r.adjusted(0, 6, -6, 0));
                button->setGeometry(r.adjusted(0, 0, -6, -6));
            }
            return false;
        }
        QWidget *container;
        QLabel *shadow;
        QPushButton *button;
    };
    auto *helper = new ResizeHelper(container, shadow, button);
    container->installEventFilter(helper);

    outButton = button;
    return container;
}

// ---------------------------------------------------------
// MainWindow
// ---------------------------------------------------------
class MainWindow : public QWidget {
public:
    MainWindow() {
        setWindowTitle("Pomeowdoro");
        resize(1260, 705);
        setStyleSheet("background-color: #6E2A45;");

        auto *root = new QVBoxLayout(this);
        root->setContentsMargins(60, 50, 60, 30);
        root->setSpacing(0);

        // --- Top row: cat icon + title ---
        auto *topRow = new QHBoxLayout();
        topRow->setSpacing(30);

        auto *cat = new CatIconWidget(this);
        topRow->addWidget(cat, 0, Qt::AlignTop);

        auto *title = new NeonTitleWidget("Pomeowdoro APP", this);
        topRow->addWidget(title, 0, Qt::AlignVCenter);
        topRow->addStretch();

        root->addLayout(topRow);
        root->addStretch();

        // --- Buttons ---
        QPushButton *btn1 = nullptr;
        QPushButton *btn2 = nullptr;

        auto *btnCol = new QVBoxLayout();
        btnCol->setSpacing(28);

        QWidget *pill1 = makePillButton("Get start for free", btn1);
        QWidget *pill2 = makePillButton("Already have an account?", btn2);
        btnCol->addWidget(pill1);
        btnCol->addWidget(pill2);

        root->addLayout(btnCol);
        root->addStretch();

        // --- Page number ---
        auto *pageRow = new QHBoxLayout();
        pageRow->addStretch();
        auto *pageLabel = new QLabel("10", this);
        pageLabel->setStyleSheet("color: #2b2b2b; font-size: 14px;");
        pageRow->addWidget(pageLabel);
        root->addLayout(pageRow);

        connect(btn1, &QPushButton::clicked, this, [](){ /* hook up "get started" action */ });
        connect(btn2, &QPushButton::clicked, this, [](){ /* hook up "sign in" action */ });
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}