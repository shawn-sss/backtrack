#ifndef NOTIFICATIONCONFIGMANAGER_H
#define NOTIFICATIONCONFIGMANAGER_H

// Qt includes
#include <QObject>
#include <QList>

// Project includes
#include "NotificationConfigStruct.h"

class NotificationConfigManager : public QObject {
    Q_OBJECT

public:
    // Singleton access
    static NotificationConfigManager& instance();

    // Initializes default notification file with welcome message
    static void initializeDefaults();

    // Loads and saves notifications
    void load();
    void save();

    // Adds a new notification
    void addNotification(const QString& message);

    // Marks all notifications as read
    void markAllAsRead();

    // Returns unread notifications
    QList<NotificationConfigStruct> unreadNotifications() const;

    // Returns all notifications
    const QList<NotificationConfigStruct>& allNotifications() const;

signals:
    void notificationsUpdated();

private:
    NotificationConfigManager();

    // Returns full path to notification file
    QString notificationFilePath() const;

    QList<NotificationConfigStruct> notifications;
};

#endif // NOTIFICATIONCONFIGMANAGER_H
