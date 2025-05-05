#ifndef NOTIFICATIONSDIALOG_H
#define NOTIFICATIONSDIALOG_H

// Qt includes
#include <QDialog>
#include <QList>

// Project includes
#include "../../config/configmanagers/notificationsmanager/notificationstruct.h"

// Forward Declaration (Qt classes)
class QListWidget;
class QPushButton;

// Dialog for displaying a list of user notifications
class NotificationsDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotificationsDialog(const QList<AppNotification>& notifications, QWidget* parent = nullptr);
    ~NotificationsDialog() override;

private:
    QListWidget* listWidget;
    QPushButton* closeButton;
};

#endif // NOTIFICATIONSDIALOG_H
