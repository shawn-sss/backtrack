// Project includes
#include "notificationsmanager.h"
#include "notificationstruct.h"
#include "../../configdirector/configdirector.h"
#include "../../../config/configsettings/notifications_settings.h"
#include "../../../core/utils/file_utils/jsonmanager.h"

// Qt includes
#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

// Constructor that loads existing notifications from file
NotificationsManager::NotificationsManager() {
    load();
}

// Returns the singleton instance of NotificationsManager
NotificationsManager& NotificationsManager::instance() {
    static NotificationsManager instance;
    return instance;
}

// Initializes the default welcome notification on first run
void NotificationsManager::initializeDefaults() {
    const QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
    + "/" + AppConfig::k_APPDATA_SETUP_FOLDER + "/"
        + NotificationSettings::k_NOTIFICATION_FILE_NAME;

    QFile file(path);
    if (!file.exists()) {
        AppNotification welcome{
            NotificationSettings::k_DEFAULT_WELCOME_MESSAGE,
            QDateTime::currentDateTimeUtc(),
            false
        };
        QJsonArray array;
        array.append(welcome.toJson());
        JsonManager::saveJsonFile(path, QJsonDocument(array));
    }
}

// Returns the full file path to the notification storage file
QString NotificationsManager::notificationFilePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
    + "/" + AppConfig::k_APPDATA_SETUP_FOLDER + "/"
        + NotificationSettings::k_NOTIFICATION_FILE_NAME;
}

// Loads notifications from the storage file into memory
void NotificationsManager::load() {
    notifications.clear();

    QJsonDocument doc = JsonManager::loadJsonFile(notificationFilePath());
    if (!doc.isArray()) {
        return;
    }

    const QJsonArray array = doc.array();
    for (const auto& val : array) {
        if (val.isObject()) {
            notifications.append(AppNotification::fromJson(val.toObject()));
        }
    }
}

// Saves the current notification list to disk
void NotificationsManager::save() {
    QJsonArray array;
    for (const AppNotification& notif : notifications) {
        array.append(notif.toJson());
    }

    JsonManager::saveJsonFile(notificationFilePath(), QJsonDocument(array));
}

// Adds a new notification message and emits update signal
void NotificationsManager::addNotification(const QString& message) {
    AppNotification n { message, QDateTime::currentDateTimeUtc(), false };
    notifications.prepend(n);
    save();
    emit notificationsUpdated();
}

// Returns a list of unread notifications
QList<AppNotification> NotificationsManager::unreadNotifications() const {
    QList<AppNotification> unread;
    for (const AppNotification& n : notifications) {
        if (!n.read) unread.append(n);
    }
    return unread;
}

// Returns the full list of notifications
QList<AppNotification> NotificationsManager::allNotifications() const {
    return notifications;
}

// Marks all notifications as read and emits update signal
void NotificationsManager::markAllAsRead() {
    for (AppNotification& n : notifications) {
        n.read = true;
    }
    save();
    emit notificationsUpdated();
}
