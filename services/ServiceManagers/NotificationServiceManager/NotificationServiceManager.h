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

    // Initializes default notification file
    static void initializeDefaults();

    // Loads and saves notification data
    void load();
    void save();

    // Adds, clears, and marks notifications
    void addNotification(const QString& message);
    void markAllAsRead();
    void clearAllNotifications();

    // Accessors
    QList<NotificationServiceStruct> unreadNotifications() const;
    const QList<NotificationServiceStruct>& allNotifications() const;
    void suspendNotifications(bool suspend);


signals:
    void notificationsUpdated();

private:
    bool notificationsSuspended = false;
    NotificationServiceManager();
    QString notificationFilePath() const;

    QList<NotificationServiceStruct> notifications;
};

#endif // NOTIFICATIONSERVICEMANAGER_H
