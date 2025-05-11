#ifndef NOTIFICATIONSDIALOG_H
#define NOTIFICATIONSDIALOG_H

// Project includes
#include "../../config/ConfigManagers/NotificationConfigManager/NotificationConfigStruct.h"

// Qt includes
#include <QDialog>
#include <QList>

// Forward Declaration (Qt class)
class QListWidget;
class QPushButton;

// Dialog class for displaying notifications
class NotificationsDialog : public QDialog {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit NotificationsDialog(const QList<NotificationConfigStruct>& notifications, QWidget* parent = nullptr);
    ~NotificationsDialog() override;

private:
    // UI elements
    QListWidget* listWidget;
    QPushButton* closeButton;
};

#endif // NOTIFICATIONSDIALOG_H
