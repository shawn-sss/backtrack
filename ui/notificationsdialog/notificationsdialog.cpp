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

// Constructs and displays the notifications dialog
NotificationsDialog::NotificationsDialog(const QList<NotificationServiceStruct>& notifications, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Notifications");
    resize(400, 300);

    auto* layout = new QVBoxLayout(this);

    listWidget = new QListWidget(this);
    listWidget->setWordWrap(true);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    closeButton = new QPushButton("Close", this);
    closeButton->setToolTip("Close the notifications dialog");
    closeButton->setCursor(Qt::PointingHandCursor);

    clearAllButton = new QPushButton("Clear All", this);
    clearAllButton->setToolTip("Delete all notifications");
    clearAllButton->setCursor(Qt::PointingHandCursor);

    QList<NotificationServiceStruct> sortedNotifications = notifications;
    std::sort(sortedNotifications.begin(), sortedNotifications.end(),
              [](const NotificationServiceStruct& a, const NotificationServiceStruct& b) {
                  return a.timestamp > b.timestamp;
              });

    for (const auto& notif : sortedNotifications) {
        const QString dateStr = notif.timestamp.toLocalTime().toString("MMM d, yyyy - h:mm AP");
        const QString badge = notif.read ? "" : "🔴 ";
        const QString text = QString("%1%2\n%3").arg(badge, dateStr, notif.message);

        auto* item = new QListWidgetItem(text);
        if (notif.read) {
            item->setForeground(QBrush(Qt::gray));
        } else {
            QFont font;
            font.setBold(true);
            item->setFont(font);
        }

        listWidget->addItem(item);
    }

    if (sortedNotifications.isEmpty()) {
        listWidget->addItem(new QListWidgetItem("No notifications."));
    } else {
        listWidget->setCurrentRow(0);
        listWidget->scrollToItem(listWidget->item(0), QAbstractItemView::PositionAtTop);
    }

    layout->addWidget(listWidget);
    layout->addWidget(clearAllButton);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &NotificationsDialog::accept);

    connect(clearAllButton, &QPushButton::clicked, this, [this]() {
        if (QMessageBox::question(this, "Confirm", "Clear all notifications?") == QMessageBox::Yes) {
            NotificationServiceManager::instance().clearAllNotifications();
            accept();
        }
    });
}

// Destructor
NotificationsDialog::~NotificationsDialog() = default;
