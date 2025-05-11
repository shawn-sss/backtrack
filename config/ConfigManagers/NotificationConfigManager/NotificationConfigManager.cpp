// Project includes
#include "NotificationConfigManager.h"
#include "NotificationConfigConstants.h"
#include "../../../core/utils/file_utils/jsonmanager.h"

// Qt includes
#include <QStandardPaths>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

// Constructor loads notifications
NotificationConfigManager::NotificationConfigManager() {
    load();
}

// Returns singleton instance
NotificationConfigManager& NotificationConfigManager::instance() {
    static NotificationConfigManager instance;
    return instance;
}

// Returns full path to the notification file
QString NotificationConfigManager::notificationFilePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" +
           NotificationSettings::kNotificationFolder + "/" +
           NotificationSettings::k_NOTIFICATION_FILE_NAME;
}

// Initializes file with default welcome message
void NotificationConfigManager::initializeDefaults() {
    const QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" +
                         NotificationSettings::kNotificationFolder + "/" +
                         NotificationSettings::k_NOTIFICATION_FILE_NAME;

    QFile file(path);
    if (!file.exists()) {
        NotificationConfigStruct welcome {
            NotificationSettings::k_DEFAULT_WELCOME_MESSAGE,
            QDateTime::currentDateTimeUtc(),
            false
        };
        QJsonArray array { welcome.toJson() };
        JsonManager::saveJsonFile(path, QJsonDocument(array));
    }
}

// Loads notifications from file
void NotificationConfigManager::load() {
    notifications.clear();

    const QJsonDocument doc = JsonManager::loadJsonFile(notificationFilePath());
    if (!doc.isArray()) return;

    const QJsonArray array = doc.array();
    notifications.reserve(array.size());
    for (const auto& val : array) {
        if (val.isObject()) {
            notifications.append(NotificationConfigStruct::fromJson(val.toObject()));
        }
    }
}

// Saves all notifications to file
void NotificationConfigManager::save() {
    QJsonArray array;
    for (const auto& notif : notifications) {
        array.append(notif.toJson());
    }
    JsonManager::saveJsonFile(notificationFilePath(), QJsonDocument(array));
}

// Adds a new unread notification
void NotificationConfigManager::addNotification(const QString& message) {
    notifications.prepend({ message, QDateTime::currentDateTimeUtc(), false });
    save();
    emit notificationsUpdated();
}

// Marks all notifications as read
void NotificationConfigManager::markAllAsRead() {
    for (auto& notif : notifications) {
        notif.read = true;
    }
    save();
    emit notificationsUpdated();
}

// Returns list of unread notifications
QList<NotificationConfigStruct> NotificationConfigManager::unreadNotifications() const {
    QList<NotificationConfigStruct> unread;
    for (const auto& notif : notifications) {
        if (!notif.read) unread.append(notif);
    }
    return unread;
}

// Returns all notifications
const QList<NotificationConfigStruct>& NotificationConfigManager::allNotifications() const {
    return notifications;
}
