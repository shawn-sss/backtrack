#ifndef NOTIFICATIONSDIALOG_H
#define NOTIFICATIONSDIALOG_H

// Project includes
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h"

// Qt includes
#include <QDialog>
#include <QList>

// Forward declaration (Qt class)
class QListWidget;
class QPushButton;

// NotificationsDialog displays and manages a list of recent notifications
class NotificationsDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotificationsDialog(const QList<NotificationServiceStruct>& notifications, QWidget* parent = nullptr);

private:
    // UI elements
    QListWidget* listWidget;
    QPushButton* closeButton;
    QPushButton* clearAllButton;
};

#endif // NOTIFICATIONSDIALOG_H
