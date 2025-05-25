#ifndef NOTIFICATIONSDIALOGSTYLING_H
#define NOTIFICATIONSDIALOGSTYLING_H

// Qt includes
#include <QString>
#include <QListWidgetItem>
#include <QBrush>
#include <QFont>
#include <QDateTime>

namespace NotificationsDialogStyling {

// Style for QListWidget items
inline constexpr auto kListWidgetStyle = R"(
    QListWidget::item {
        margin: 0px;
        padding: 2px 6px;
        border: none;
        font-size: 13px;
        font-family: 'Segoe UI', 'Arial', sans-serif;
        color: #dddddd;
    }
    QListWidget::item:selected {
        background-color: #2a3b4c;
    }
)";

// Formats the text for each notification item
inline QString formatNotificationText(const NotificationServiceStruct& notif) {
    QString dateStr = notif.timestamp.toLocalTime().toString("MMM d, yyyy - h:mm AP");
    QString badge = notif.read ? "" : "🔴 ";
    return QString("%1%2\n%3").arg(badge, dateStr, notif.message);
}

// Creates a styled list widget item based on read state
inline QListWidgetItem* createNotificationItem(const NotificationServiceStruct& notif) {
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

} // namespace NotificationsDialogStyling

#endif // NOTIFICATIONSDIALOGSTYLING_H
