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

    // Initializes default notification file
    static void initializeDefaults();

    // Persistence
    void load();
    void save();

    // Notification management
    void addNotification(const QString& message);
    void markAllAsRead();
    void clearAllNotifications();

    // Notification queries
    QList<NotificationServiceStruct> unreadNotifications() const;
    const QList<NotificationServiceStruct>& allNotifications() const;

signals:
    void notificationsUpdated();

private:
    NotificationServiceManager();

    // Internal path resolution
    QString notificationFilePath() const;

    // Internal storage
    QList<NotificationServiceStruct> notifications;
};

#endif // NOTIFICATIONSERVICEMANAGER_H
