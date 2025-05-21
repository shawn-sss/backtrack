#include "NotificationServiceManager.h"
#include "NotificationServiceConstants.h"
#include "../../../../core/shared/jsonmanager.h"

// Qt includes
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>

// Constructs and loads existing notifications
NotificationServiceManager::NotificationServiceManager() {
    load();
}

// Returns the singleton instance of the notification manager
NotificationServiceManager& NotificationServiceManager::instance() {
    static NotificationServiceManager instance;
    return instance;
}

// Returns the full path to the notification JSON file
QString NotificationServiceManager::notificationFilePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" +
           NotificationSettings::kNotificationFolder + "/" +
           NotificationSettings::kNotificationFileName;
}

// Initializes notification file with a welcome message if not present or empty
void NotificationServiceManager::initializeDefaults() {
    const QString path = instance().notificationFilePath();

    QFile file(path);
    bool needsInit = !file.exists();
    if (!needsInit && file.open(QIODevice::ReadOnly)) {
        needsInit = file.size() == 0;
        file.close();
    }

    if (!needsInit) return;

    NotificationServiceStruct welcome{
        NotificationSettings::k_DEFAULT_WELCOME_MESSAGE,
        QDateTime::currentDateTimeUtc(),
        false
    };

    QJsonArray array{ welcome.toJson() };
    bool success = JsonManager::saveJsonFile(path, QJsonDocument(array));

    if (success) {
        instance().load();
    }
}

// Loads notifications from disk
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

// Saves all notifications to disk
void NotificationServiceManager::save() {
    if (notificationsSuspended) return;

    QJsonArray array;
    const auto& list = notifications;

    for (int i = 0; i < list.size(); ++i) {
        array.append(list.at(i).toJson());
    }

    QJsonDocument doc(array);
    JsonManager::saveJsonFile(notificationFilePath(), doc);
}

// Adds a new unread notification
void NotificationServiceManager::addNotification(const QString& message) {
    if (notificationsSuspended) return;

    notifications.prepend({ message, QDateTime::currentDateTimeUtc(), false });
    save();
    emit notificationsUpdated();
}

// Marks all notifications as read
void NotificationServiceManager::markAllAsRead() {
    if (notificationsSuspended) return;

    for (auto& n : notifications) {
        n.read = true;
    }
    save();
    emit notificationsUpdated();
}

// Returns a list of unread notifications
QList<NotificationServiceStruct> NotificationServiceManager::unreadNotifications() const {
    QList<NotificationServiceStruct> unread;
    for (const auto& n : notifications) {
        if (!n.read)
            unread.append(n);
    }
    return unread;
}

// Returns all stored notifications
const QList<NotificationServiceStruct>& NotificationServiceManager::allNotifications() const {
    return notifications;
}

// Clears all notifications and resets with welcome message
void NotificationServiceManager::clearAllNotifications() {
    if (notificationsSuspended) return;

    notifications.clear();
    notifications.append({
        NotificationSettings::k_DEFAULT_WELCOME_MESSAGE,
        QDateTime::currentDateTimeUtc(),
        false
    });
    save();
    emit notificationsUpdated();
}

// Suspends or resumes all notifications and file writes
void NotificationServiceManager::suspendNotifications(bool suspend) {
    notificationsSuspended = suspend;
}
