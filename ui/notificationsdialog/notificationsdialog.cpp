// Project includes
#include "notificationsdialog.h"
#include "NotificationsDialogStyling.h"
#include "NotificationsDialogConstants.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"

// Qt includes
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QAbstractScrollArea>
#include <QMessageBox>

// C++ includes
#include <algorithm>

// Constructs the notifications dialog and sets up UI and logic
NotificationsDialog::NotificationsDialog(const QList<NotificationServiceStruct>& notifications, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(NotificationsDialogConstants::kWindowTitle);
    resize(NotificationsDialogConstants::kDialogWidth, NotificationsDialogConstants::kDialogHeight);

    auto* layout = new QVBoxLayout(this);

    listWidget = new QListWidget(this);
    listWidget->setWordWrap(true);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    listWidget->setStyleSheet(NotificationsDialogStyling::kListWidgetStyle);

    clearAllButton = new QPushButton(NotificationsDialogConstants::kClearAllText, this);
    clearAllButton->setToolTip(NotificationsDialogConstants::kClearAllTooltip);
    clearAllButton->setCursor(Qt::PointingHandCursor);

    closeButton = new QPushButton(NotificationsDialogConstants::kCloseText, this);
    closeButton->setToolTip(NotificationsDialogConstants::kCloseTooltip);
    closeButton->setCursor(Qt::PointingHandCursor);

    QList<NotificationServiceStruct> sortedNotifications = notifications;
    std::sort(sortedNotifications.begin(), sortedNotifications.end(),
              [](const NotificationServiceStruct& a, const NotificationServiceStruct& b) {
                  return a.timestamp > b.timestamp;
              });

    if (sortedNotifications.isEmpty()) {
        listWidget->addItem(new QListWidgetItem(NotificationsDialogConstants::kNoNotificationsText));
    } else {
        for (const auto& notif : sortedNotifications) {
            listWidget->addItem(NotificationsDialogStyling::createNotificationItem(notif));
        }
        listWidget->setCurrentRow(0);
        listWidget->scrollToItem(listWidget->item(0), QAbstractItemView::PositionAtTop);
    }

    layout->addWidget(listWidget);
    layout->addWidget(clearAllButton);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &NotificationsDialog::accept);

    connect(clearAllButton, &QPushButton::clicked, this, [this]() {
        const QMessageBox::StandardButton reply = QMessageBox::warning(
            this,
            NotificationsDialogConstants::kConfirmTitle,
            NotificationsDialogConstants::kConfirmMessage,
            QMessageBox::Yes | QMessageBox::Cancel
            );

        if (reply == QMessageBox::Yes) {
            NotificationServiceManager::instance().clearAllNotifications();
            QMessageBox::information(
                this,
                NotificationsDialogConstants::kClearedTitle,
                NotificationsDialogConstants::kClearedMessage
                );
            accept();
        }
    });
}
