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

// Lifecycle: construct and load
NotificationServiceManager::NotificationServiceManager() {
    load();
}

// Lifecycle: singleton instance
NotificationServiceManager& NotificationServiceManager::instance() {
    static NotificationServiceManager instance;
    return instance;
}

// Persistence: file path
QString NotificationServiceManager::notificationFilePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" +
           NotificationSettings::kNotificationFolder + "/" +
           NotificationSettings::kNotificationFileName;
}

// Defaults: initialize with welcome message
void NotificationServiceManager::initializeDefaults() {
    const QString path = instance().notificationFilePath();

    QFile file(path);
    bool needsInit = !file.exists();
    if (!needsInit && file.open(QIODevice::ReadOnly)) {
        needsInit = (file.size() == 0);
        file.close();
    }
    if (!needsInit) return;

    NotificationServiceStruct welcome{
        NotificationSettings::kDefaultWelcomeMessage,
        QDateTime::currentDateTimeUtc(),
        false
    };

    const QJsonArray array{ welcome.toJson() };
    if (JsonManager::saveJsonFile(path, QJsonDocument(array))) {
        instance().load();
    }
}

// Persistence: load notifications
void NotificationServiceManager::load() {
    notifications.clear();

    const QString path = notificationFilePath();
    const QJsonDocument doc = JsonManager::loadJsonFile(path);
    if (!doc.isArray()) return;

    const QJsonArray array = doc.array();
    for (const auto& val : array) {
        if (val.isObject()) {
            notifications.append(NotificationServiceStruct::fromJson(val.toObject()));
        }
    }
}

// Persistence: save notifications
void NotificationServiceManager::save() {
    if (notificationsSuspended) return;

    QJsonArray array;
    for (const auto& n : notifications) {
        array.append(n.toJson());
    }
    JsonManager::saveJsonFile(notificationFilePath(), QJsonDocument(array));
}

// Mutation: add notification
void NotificationServiceManager::addNotification(const QString& message) {
    if (notificationsSuspended) return;

    notifications.prepend({ message, QDateTime::currentDateTimeUtc(), false });
    save();
    emit notificationsUpdated();
}

// Mutation: mark all as read
void NotificationServiceManager::markAllAsRead() {
    if (notificationsSuspended) return;

    for (auto& n : notifications) {
        n.read = true;
    }
    save();
    emit notificationsUpdated();
}

// Query: unread notifications
QList<NotificationServiceStruct> NotificationServiceManager::unreadNotifications() const {
    QList<NotificationServiceStruct> unread;
    for (const auto& n : notifications) {
        if (!n.read) unread.append(n);
    }
    return unread;
}

// Query: all notifications
const QList<NotificationServiceStruct>& NotificationServiceManager::allNotifications() const {
    return notifications;
}

// Mutation: clear all notifications
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

// Control: suspend/resume notifications
void NotificationServiceManager::suspendNotifications(bool suspend) {
    notificationsSuspended = suspend;
}
