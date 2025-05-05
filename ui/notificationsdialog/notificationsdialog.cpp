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

// Constructor for NotificationsDialog that initializes the dialog with notification items
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

    for (const auto& notif : notifications) {
        QString dateStr = notif.timestamp.toLocalTime().toString("MMM d, yyyy - h:mm AP");
        QString text = QString("%1\n%2").arg(dateStr, notif.message);

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
        auto* item = new QListWidgetItem("No notifications.");
        listWidget->addItem(item);
    }

    layout->addWidget(listWidget);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &NotificationsDialog::accept);
}

// Destructor for NotificationsDialog
NotificationsDialog::~NotificationsDialog() = default;
