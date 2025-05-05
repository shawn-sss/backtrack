#ifndef NOTIFICATIONSTRUCT_H
#define NOTIFICATIONSTRUCT_H

// Qt includes
#include <QDateTime>
#include <QJsonObject>
#include <QString>

// Project includes
#include "../../configsettings/_settings.h"

// Represents a single user-facing notification
struct AppNotification {
    QString message;
    QDateTime timestamp;
    bool read;

    // Converts the notification to a QJsonObject
    QJsonObject toJson() const {
        return {
            {NotificationSettings::Fields::k_MESSAGE, message},
            {NotificationSettings::Fields::k_TIMESTAMP, timestamp.toString(Qt::ISODate)},
            {NotificationSettings::Fields::k_READ, read}
        };
    }

    // Creates a notification from a QJsonObject
    static AppNotification fromJson(const QJsonObject& obj) {
        return {
            obj.value(NotificationSettings::Fields::k_MESSAGE).toString(),
            QDateTime::fromString(obj.value(NotificationSettings::Fields::k_TIMESTAMP).toString(), Qt::ISODate),
            obj.value(NotificationSettings::Fields::k_READ).toBool()
        };
    }
};

#endif // NOTIFICATIONSTRUCT_H
