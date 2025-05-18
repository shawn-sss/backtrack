// Project includes
#include "notificationsdialog.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"

// Qt includes
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QAbstractScrollArea>
#include <QBrush>
#include <QFont>
#include <QDateTime>
#include <QMessageBox>

// C++ includes
#include <algorithm>

// Formats the text for each notification item
namespace {
QString formatNotificationText(const NotificationServiceStruct& notif) {
    QString dateStr = notif.timestamp.toLocalTime().toString("MMM d, yyyy - h:mm AP");
    QString badge = notif.read ? "" : "🔴 ";
    return QString("%1%2\n%3").arg(badge, dateStr, notif.message);
}

// Creates a styled list widget item based on read state
QListWidgetItem* createNotificationItem(const NotificationServiceStruct& notif) {
    QListWidgetItem* item = new QListWidgetItem(formatNotificationText(notif));
    if (notif.read) {
        item->setForeground(QBrush(Qt::gray));
    } else {
        QFont font;
        font.setBold(true);
        item->setFont(font);
    }
    return item;
}
}

// Constructs the notifications dialog with populated notification items
NotificationsDialog::NotificationsDialog(const QList<NotificationServiceStruct>& notifications, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Notifications");
    resize(400, 300);

    auto* layout = new QVBoxLayout(this);

    listWidget = new QListWidget(this);
    listWidget->setWordWrap(true);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    clearAllButton = new QPushButton("Clear All", this);
    clearAllButton->setToolTip("Delete all notifications");
    clearAllButton->setCursor(Qt::PointingHandCursor);

    closeButton = new QPushButton("Close", this);
    closeButton->setToolTip("Close the notifications dialog");
    closeButton->setCursor(Qt::PointingHandCursor);

    QList<NotificationServiceStruct> sortedNotifications = notifications;
    std::sort(sortedNotifications.begin(), sortedNotifications.end(),
              [](const NotificationServiceStruct& a, const NotificationServiceStruct& b) {
                  return a.timestamp > b.timestamp;
              });

    if (sortedNotifications.isEmpty()) {
        listWidget->addItem(new QListWidgetItem("No notifications."));
    } else {
        for (const auto& notif : sortedNotifications) {
            listWidget->addItem(createNotificationItem(notif));
        }
        listWidget->setCurrentRow(0);
        listWidget->scrollToItem(listWidget->item(0), QAbstractItemView::PositionAtTop);
    }

    layout->addWidget(listWidget);
    layout->addWidget(clearAllButton);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &NotificationsDialog::accept);

    connect(clearAllButton, &QPushButton::clicked, this, [this]() {
        const QMessageBox::StandardButton reply = QMessageBox::warning(
            this,
            "Confirm Deletion",
            "This will delete all notifications permanently. Are you sure?",
            QMessageBox::Yes | QMessageBox::Cancel
            );

        if (reply == QMessageBox::Yes) {
            NotificationServiceManager::instance().clearAllNotifications();
            QMessageBox::information(this, "Cleared", "All notifications have been deleted.");
            accept();
        }
    });
}
