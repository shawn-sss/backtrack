#ifndef NOTIFICATIONSDIALOG_H
#define NOTIFICATIONSDIALOG_H

// Project includes
#include "../../config/configmanagers/notificationsmanager/notificationstruct.h"

// Qt includes
#include <QDialog>
#include <QList>

// Forward Declaration (Qt class)
class QListWidget;
class QPushButton;

// Dialog class for displaying user notifications
class NotificationsDialog : public QDialog {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit NotificationsDialog(const QList<AppNotification>& notifications, QWidget* parent = nullptr);
    ~NotificationsDialog() override;

private:
    // UI elements for displaying notifications and closing the dialog
    QListWidget* listWidget;
    QPushButton* closeButton;
};

#endif // NOTIFICATIONSDIALOG_H
