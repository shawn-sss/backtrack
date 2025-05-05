// Project includes
#include "notificationsdialog.h"

// Qt includes
#include <QBrush>
#include <QDateTime>
#include <QFont>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

// Constructs the notifications dialog and populates the list
NotificationsDialog::NotificationsDialog(const QList<AppNotification>& notifications, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Notifications");
    resize(400, 300);

    auto* layout = new QVBoxLayout(this);

    listWidget = new QListWidget(this);
    closeButton = new QPushButton("Close", this);

    for (const auto& notif : notifications) {
        QString text = QString("[%1] %2")
        .arg(notif.timestamp.toLocalTime().toString("MMM d, yyyy - h:mm AP"))
            .arg(notif.message);

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

    if (notifications.isEmpty()) {
        listWidget->addItem("No notifications.");
    }

    layout->addWidget(listWidget);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &NotificationsDialog::accept);
}

// Destroys the notifications dialog
NotificationsDialog::~NotificationsDialog() = default;
