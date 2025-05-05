#include "notificationsdialog.h"

// Qt includes
#include <QBrush>
#include <QDateTime>
#include <QFont>
#include <QFontMetrics>
#include <QListWidget>
#include <QPushButton>
#include <QScrollBar>
#include <QVBoxLayout>
#include <algorithm>

NotificationsDialog::NotificationsDialog(const QList<AppNotification>& notifications, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Notifications");
    resize(400, 300);

    auto* layout = new QVBoxLayout(this);

    listWidget = new QListWidget(this);
    closeButton = new QPushButton("Close", this);

    closeButton->setToolTip("Close the notifications dialog");
    closeButton->setCursor(Qt::PointingHandCursor);

    listWidget->setWordWrap(true);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    QList<AppNotification> sortedNotifications = notifications;
    std::sort(sortedNotifications.begin(), sortedNotifications.end(),
              [](const AppNotification& a, const AppNotification& b) {
                  return a.timestamp > b.timestamp;
              });

    for (const auto& notif : sortedNotifications) {
        QString dateStr = notif.timestamp.toLocalTime().toString("MMM d, yyyy - h:mm AP");
        QString badge = notif.read ? "" : "🔴 ";
        QString text = QString("%1%2\n%3").arg(badge, dateStr, notif.message);

        auto* item = new QListWidgetItem(text);

        if (!notif.read) {
            QFont font;
            font.setBold(true);
            item->setFont(font);
        } else {
            item->setForeground(QBrush(Qt::gray));
        }

        listWidget->addItem(item);
    }

    if (sortedNotifications.isEmpty()) {
        auto* item = new QListWidgetItem("No notifications.");
        listWidget->addItem(item);
    }

    if (!sortedNotifications.isEmpty()) {
        auto* latestItem = listWidget->item(0);
        listWidget->setCurrentItem(latestItem);
        listWidget->scrollToItem(latestItem, QAbstractItemView::PositionAtTop);
    }

    layout->addWidget(listWidget);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &NotificationsDialog::accept);
}

NotificationsDialog::~NotificationsDialog() = default;
