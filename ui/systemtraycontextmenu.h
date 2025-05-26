#ifndef SYSTEMTRAYCONTEXTMENU_H
#define SYSTEMTRAYCONTEXTMENU_H

// Project includes

// Qt includes
#include <QMenu>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QAction>

// C++ includes

// Forward declaration (Custom class)

// Forward declaration (Qt class)

class SystemTrayContextMenu : public QMenu {
    Q_OBJECT

public:
    explicit SystemTrayContextMenu(QWidget* parent = nullptr)
        : QMenu(parent)
    {
        connect(this, &QMenu::triggered, this, &SystemTrayContextMenu::onActionTriggered);
    }

    void popup(const QPoint& pos, QAction* atAction = nullptr)
    {
        popupTimer.start();
        suppressionUsed = false;
        QMenu::popup(pos, atAction);
    }

signals:
    void safeTriggered(QAction* action);

protected:
    void mouseReleaseEvent(QMouseEvent* e) override
    {
        if (!suppressionUsed && popupTimer.elapsed() < 250) {
            suppressionUsed = true;
            e->accept();
            return;
        }

        QMenu::mouseReleaseEvent(e);
    }

private slots:
    void onActionTriggered(QAction* action)
    {
        if (popupTimer.elapsed() < 300) {
            return;
        }

        emit safeTriggered(action);
    }

private:
    QElapsedTimer popupTimer;
    bool suppressionUsed = false;
};

#endif // SYSTEMTRAYCONTEXTMENU_H
