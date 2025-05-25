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

    // Convert struct to JSON object
    QJsonObject toJson() const {
        return {
            { NotificationSettings::Fields::k_MESSAGE, message },
            { NotificationSettings::Fields::k_TIMESTAMP, timestamp.toString(Qt::ISODate) },
            { NotificationSettings::Fields::k_READ, read }
        };
    }

    // Create struct from JSON object
    static NotificationServiceStruct fromJson(const QJsonObject& obj) {
        return {
            obj.value(NotificationSettings::Fields::k_MESSAGE).toString(),
            QDateTime::fromString(obj.value(NotificationSettings::Fields::k_TIMESTAMP).toString(), Qt::ISODate),
            obj.value(NotificationSettings::Fields::k_READ).toBool()
        };
    }
};

#endif // NOTIFICATIONSERVICESTRUCT_H
