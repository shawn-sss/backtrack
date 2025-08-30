#ifndef NOTIFICATIONSERVICESTRUCT_H
#define NOTIFICATIONSERVICESTRUCT_H

// Project includes
#include "NotificationServiceConstants.h"

// Qt includes
#include <QDateTime>
#include <QJsonObject>
#include <QString>

// Represents a single notification with JSON conversion
struct NotificationServiceStruct {
    QString message;
    QDateTime timestamp;
    bool read;

    // Convert to JSON
    QJsonObject toJson() const {
        return {
            { NotificationSettings::Fields::kMessage, message },
            { NotificationSettings::Fields::kTimestamp, timestamp.toString(Qt::ISODate) },
            { NotificationSettings::Fields::kRead, read }
        };
    }

    // Construct from JSON
    static NotificationServiceStruct fromJson(const QJsonObject& obj) {
        return {
            obj.value(NotificationSettings::Fields::kMessage).toString(),
            QDateTime::fromString(obj.value(NotificationSettings::Fields::kTimestamp).toString(), Qt::ISODate),
            obj.value(NotificationSettings::Fields::kRead).toBool()
        };
    }
};

#endif
