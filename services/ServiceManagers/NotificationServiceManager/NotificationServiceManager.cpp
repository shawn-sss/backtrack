// Project includes
#include "NotificationServiceManager.h"
#include "NotificationServiceConstants.h"
#include "../../../../core/shared/jsonmanager.h"

// Qt includes
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

// Constructor loads notifications
NotificationServiceManager::NotificationServiceManager() {
    load();
}

// Returns singleton instance
NotificationServiceManager& NotificationServiceManager::instance() {
    static NotificationServiceManager instance;
    return instance;
}

// Returns full path to the notification file
QString NotificationServiceManager::notificationFilePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" +
           NotificationSettings::kNotificationFolder + "/" +
           NotificationSettings::k_NOTIFICATION_FILE_NAME;
}

// Initializes file with default welcome message
void NotificationServiceManager::initializeDefaults() {
    const QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" +
                         NotificationSettings::kNotificationFolder + "/" +
                         NotificationSettings::k_NOTIFICATION_FILE_NAME;

    QFile file(path);
    if (!file.exists()) {
        NotificationServiceStruct welcome{
            NotificationSettings::k_DEFAULT_WELCOME_MESSAGE,
            QDateTime::currentDateTimeUtc(),
            false
        };
        QJsonArray array{ welcome.toJson() };
        JsonManager::saveJsonFile(path, QJsonDocument(array));
    }
}

// Loads notifications from file
void NotificationServiceManager::load() {
    notifications.clear();

    const QJsonDocument doc = JsonManager::loadJsonFile(notificationFilePath());
    if (!doc.isArray()) return;

    const QJsonArray array = doc.array();
    notifications.reserve(array.size());
    for (const auto& val : array) {
        if (val.isObject()) {
            notifications.append(NotificationServiceStruct::fromJson(val.toObject()));
        }
    }
}

// Saves all notifications to file
void NotificationServiceManager::save() {
    QJsonArray array;
    for (const auto& notif : notifications) {
        array.append(notif.toJson());
    }
    JsonManager::saveJsonFile(notificationFilePath(), QJsonDocument(array));
}

// Adds a new unread notification
void NotificationServiceManager::addNotification(const QString& message) {
    notifications.prepend({ message, QDateTime::currentDateTimeUtc(), false });
    save();
    emit notificationsUpdated();
}

// Marks all notifications as read
void NotificationServiceManager::markAllAsRead() {
    for (auto& notif : notifications) {
        notif.read = true;
    }
    save();
    emit notificationsUpdated();
}

// Returns list of unread notifications
QList<NotificationServiceStruct> NotificationServiceManager::unreadNotifications() const {
    QList<NotificationServiceStruct> unread;
    for (const auto& notif : notifications) {
        if (!notif.read) unread.append(notif);
    }
    return unread;
}

// Returns all notifications
const QList<NotificationServiceStruct>& NotificationServiceManager::allNotifications() const {
    return notifications;
}
