#ifndef NOTIFICATIONSMANAGER_H
#define NOTIFICATIONSMANAGER_H

// Qt includes
#include <QObject>
#include <QList>
#include <QString>

// Project includes
#include "notificationstruct.h"

// Manages reading, writing, and updating user notifications
class NotificationsManager : public QObject {
    Q_OBJECT

public:
    // Singleton access
    static NotificationsManager& instance();

    // Initializes default file and welcome message if needed
    static void initializeDefaults();

    // Notification file operations
    void load();
    void save();

    // Notification modification
    void addNotification(const QString& message);
    void markAllAsRead();

    // Notification queries
    QList<AppNotification> unreadNotifications() const;
    QList<AppNotification> allNotifications() const;

signals:
    // Emitted when notifications list is changed
    void notificationsUpdated();

private:
    // Constructor for singleton pattern
    NotificationsManager();

    // Computes full path to notification storage file
    QString notificationFilePath() const;

    QList<AppNotification> notifications;
};

#endif // NOTIFICATIONSMANAGER_H
