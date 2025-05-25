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

    // Initialize default notification file
    static void initializeDefaults();

    // Load and save notification data
    void load();
    void save();

    // Modify notifications
    void addNotification(const QString& message);
    void markAllAsRead();
    void clearAllNotifications();

    // Access notification data
    QList<NotificationServiceStruct> unreadNotifications() const;
    const QList<NotificationServiceStruct>& allNotifications() const;
    void suspendNotifications(bool suspend);

signals:
    void notificationsUpdated();

private:
    NotificationServiceManager();
    QString notificationFilePath() const;

    bool notificationsSuspended = false;
    QList<NotificationServiceStruct> notifications;
};

#endif // NOTIFICATIONSERVICEMANAGER_H
