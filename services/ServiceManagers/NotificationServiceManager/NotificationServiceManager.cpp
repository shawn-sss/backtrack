// Project includes
#include "NotificationServiceManager.h"
#include "NotificationServiceConstants.h"
#include "../JsonServiceManager/JsonServiceManager.h"

// Qt includes
#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

// Construct and load manager
NotificationServiceManager::NotificationServiceManager() {
    load();
}

// Return singleton instance
NotificationServiceManager& NotificationServiceManager::instance() {
    static NotificationServiceManager instance;
    return instance;
}

// Return notification file path
QString NotificationServiceManager::notificationFilePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" +
           NotificationSettings::kNotificationFolder + "/" +
           NotificationSettings::kNotificationFileName;
}

// Initialize with default welcome message
void NotificationServiceManager::initializeDefaults() {
    const QString path = instance().notificationFilePath();

    QFile file(path);
    bool needsInit = !file.exists();
    if (!needsInit && file.open(QIODevice::ReadOnly)) {
        needsInit = (file.size() == 0);
        file.close();
    }
    if (!needsInit) return;

    NotificationSettings::NotificationServiceStruct welcome{
        NotificationSettings::kDefaultWelcomeMessage,
        QDateTime::currentDateTimeUtc(),
        false
    };

    const QJsonArray array{ welcome.toJson() };
    if (JsonManager::saveJsonFile(path, QJsonDocument(array))) {
        instance().load();
    }
}

// Load notifications from file
void NotificationServiceManager::load() {
    notifications.clear();

    const QString path = notificationFilePath();
    const QJsonDocument doc = JsonManager::loadJsonFile(path);
    if (!doc.isArray()) return;

    const QJsonArray array = doc.array();
    for (const auto& val : array) {
        if (val.isObject()) {
            notifications.append(
                NotificationSettings::NotificationServiceStruct::fromJson(val.toObject())
                );
        }
    }
}

// Save notifications to file
void NotificationServiceManager::save() {
    if (notificationsSuspended) return;

    QJsonArray array;
    for (const auto& n : notifications) {
        array.append(n.toJson());
    }
    JsonManager::saveJsonFile(notificationFilePath(), QJsonDocument(array));
}

// Add a new notification
void NotificationServiceManager::addNotification(const QString& message) {
    if (notificationsSuspended) return;

    notifications.prepend({
        message,
        QDateTime::currentDateTimeUtc(),
        false
    });
    save();
    emit notificationsUpdated();
}

// Mark all notifications as read
void NotificationServiceManager::markAllAsRead() {
    if (notificationsSuspended) return;

    for (auto& n : notifications) {
        n.read = true;
    }
    save();
    emit notificationsUpdated();
}

// Return unread notifications
QList<NotificationSettings::NotificationServiceStruct> NotificationServiceManager::unreadNotifications() const {
    QList<NotificationSettings::NotificationServiceStruct> unread;
    for (const auto& n : notifications) {
        if (!n.read) unread.append(n);
    }
    return unread;
}

// Return all notifications
const QList<NotificationSettings::NotificationServiceStruct>& NotificationServiceManager::allNotifications() const {
    return notifications;
}

// Clear all notifications and reset with welcome
void NotificationServiceManager::clearAllNotifications() {
    if (notificationsSuspended) return;

    notifications.clear();
    notifications.append({
        NotificationSettings::kDefaultWelcomeMessage,
        QDateTime::currentDateTimeUtc(),
        false
    });
    save();
    emit notificationsUpdated();
}

// Suspend or resume notifications
void NotificationServiceManager::suspendNotifications(bool suspend) {
    notificationsSuspended = suspend;
}
