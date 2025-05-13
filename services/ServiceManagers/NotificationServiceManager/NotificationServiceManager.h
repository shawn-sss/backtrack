#ifndef NOTIFICATIONSERVICEMANAGER_H
#define NOTIFICATIONSERVICEMANAGER_H

// Project includes
#include "NotificationServiceStruct.h"

// Qt includes
#include <QObject>
#include <QList>
#include <QString>

// Manages creation, loading, and storage of notifications
class NotificationServiceManager : public QObject {
    Q_OBJECT

public:
    // Singleton access
    static NotificationServiceManager& instance();

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
    QList<NotificationServiceStruct> unreadNotifications() const;

    // Returns all notifications
    const QList<NotificationServiceStruct>& allNotifications() const;

signals:
    void notificationsUpdated();

private:
    NotificationServiceManager();

    // Returns full path to notification file
    QString notificationFilePath() const;

    // Stored notifications
    QList<NotificationServiceStruct> notifications;
};

#endif // NOTIFICATIONSERVICEMANAGER_H
