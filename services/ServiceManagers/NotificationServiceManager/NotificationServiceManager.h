#ifndef NOTIFICATIONSERVICEMANAGER_H
#define NOTIFICATIONSERVICEMANAGER_H

// Project includes
#include "NotificationServiceConstants.h"

// Qt includes
#include <QObject>
#include <QList>
#include <QString>

// Manages notifications for the user
class NotificationServiceManager : public QObject {
    Q_OBJECT

public:
    // Singleton access
    static NotificationServiceManager& instance();

    // Initialize with defaults
    static void initializeDefaults();

    // Persistence
    void load();
    void save();

    // Mutation
    void addNotification(const QString& message);
    void markAllAsRead();
    void clearAllNotifications();
    void suspendNotifications(bool suspend);

    // Query
    QList<NotificationSettings::NotificationServiceStruct> unreadNotifications() const;
    const QList<NotificationSettings::NotificationServiceStruct>& allNotifications() const;

signals:
    void notificationsUpdated();

private:
    NotificationServiceManager();
    QString notificationFilePath() const;

    bool notificationsSuspended = false;
    QList<NotificationSettings::NotificationServiceStruct> notifications;
};

#endif // NOTIFICATIONSERVICEMANAGER_H
