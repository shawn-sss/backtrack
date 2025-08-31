#ifndef NOTIFICATIONSDIALOG_H
#define NOTIFICATIONSDIALOG_H

// Project includes
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceConstants.h"

// Qt includes
#include <QDialog>
#include <QList>

// Forward declaration (Qt class)
class QListWidget;
class QPushButton;

// Notifications dialog window
class NotificationsDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotificationsDialog(
        const QList<NotificationSettings::NotificationServiceStruct>& notifications,
        QWidget* parent = nullptr);

private:
    QListWidget* listWidget     = nullptr;
    QPushButton* clearAllButton = nullptr;

    Q_DISABLE_COPY_MOVE(NotificationsDialog)
};

#endif // NOTIFICATIONSDIALOG_H
