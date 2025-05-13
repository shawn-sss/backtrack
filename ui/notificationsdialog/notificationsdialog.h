#ifndef NOTIFICATIONSDIALOG_H
#define NOTIFICATIONSDIALOG_H

// Project includes
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h"

// Qt includes
#include <QDialog>
#include <QList>

// Forward declarations (Qt class)
class QListWidget;
class QPushButton;

// Dialog class for displaying notifications
class NotificationsDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotificationsDialog(const QList<NotificationServiceStruct>& notifications, QWidget* parent = nullptr);
    ~NotificationsDialog() override;

private:
    QListWidget* listWidget;
    QPushButton* closeButton;
};

#endif // NOTIFICATIONSDIALOG_H
