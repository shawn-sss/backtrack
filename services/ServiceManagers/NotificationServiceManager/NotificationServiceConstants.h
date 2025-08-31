#ifndef NOTIFICATIONSERVICECONSTANTS_H
#define NOTIFICATIONSERVICECONSTANTS_H

// Project includes
#include "../../../../constants/app_info.h"

// Qt includes
#include <QDateTime>
#include <QJsonObject>
#include <QString>

namespace NotificationSettings {

// File/folder names
inline constexpr auto& kNotificationFileName = App::Items::k_APPDATA_SETUP_NOTIFICATIONS_FILE;
inline constexpr auto& kNotificationFolder   = App::Items::k_APPDATA_SETUP_FOLDER;

// Defaults
inline constexpr const char* kDefaultWelcomeMessage = "👋 Welcome! Thanks for using this app.";

// Notification entry fields
namespace Fields {
inline constexpr const char* kMessage   = "message";
inline constexpr const char* kTimestamp = "timestamp";
inline constexpr const char* kRead      = "read";
}

// Represents a single notification with JSON conversion
struct NotificationServiceStruct {
    QString message;
    QDateTime timestamp;
    bool read;

    // Convert struct to JSON
    QJsonObject toJson() const {
        return {
            { Fields::kMessage, message },
            { Fields::kTimestamp, timestamp.toString(Qt::ISODate) },
            { Fields::kRead, read }
        };
    }

    // Construct struct from JSON
    static NotificationServiceStruct fromJson(const QJsonObject& obj) {
        return {
            obj.value(Fields::kMessage).toString(),
            QDateTime::fromString(obj.value(Fields::kTimestamp).toString(), Qt::ISODate),
            obj.value(Fields::kRead).toBool()
        };
    }
};

} // namespace NotificationSettings

#endif // NOTIFICATIONSERVICECONSTANTS_H
