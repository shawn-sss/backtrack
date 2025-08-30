#ifndef NOTIFICATIONSERVICEMANAGER_H
#define NOTIFICATIONSERVICEMANAGER_H

// Project includes
#include "NotificationServiceStruct.h"

// Qt includes
#include <QObject>
#include <QList>
#include <QString>

// Manages notifications for the user
class NotificationServiceManager : public QObject {
    Q_OBJECT

public:
    static NotificationServiceManager& instance();

    // Defaults
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
    QList<NotificationServiceStruct> unreadNotifications() const;
    const QList<NotificationServiceStruct>& allNotifications() const;

signals:
    void notificationsUpdated();

private:
    NotificationServiceManager();
    QString notificationFilePath() const;

    bool notificationsSuspended = false;
    QList<NotificationServiceStruct> notifications;
};

#endif
