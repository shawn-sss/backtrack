// Project includes
#include "notificationsdialog.h"
#include "NotificationsDialogStyling.h"
#include "NotificationsDialogConstants.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"

// Qt includes
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QAbstractScrollArea>
#include <QMessageBox>

// C++ includes
#include <algorithm>

// Formats the text for each notification item
static QString formatNotificationText(const NotificationServiceStruct& notif) {
    QString dateStr = notif.timestamp.toLocalTime().toString("MMM d, yyyy - h:mm AP");
    QString badge = notif.read ? "" : "🔴 ";
    return QString("%1%2\n%3").arg(badge, dateStr, notif.message);
}

// Creates a styled list widget item based on read state
static QListWidgetItem* createNotificationItem(const NotificationServiceStruct& notif) {
    auto* item = new QListWidgetItem(formatNotificationText(notif));
    if (notif.read) {
        item->setForeground(QBrush(Qt::gray));
    } else {
        QFont font;
        font.setBold(true);
        item->setFont(font);
    }
    return item;
}

// Constructor: sets up UI and behavior for notifications dialog
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
    closeButton = new QPushButton(NotificationsDialogConstants::kCloseText, this);

    // Unified cursor + tooltip setup via shared UI service
    Shared::UI::applyButtonTooltipAndCursor(clearAllButton, NotificationsDialogConstants::kClearAllTooltip);
    Shared::UI::applyButtonTooltipAndCursor(closeButton, NotificationsDialogConstants::kCloseTooltip);

    // Sort and populate list
    QList<NotificationServiceStruct> sortedNotifications = notifications;
    std::sort(sortedNotifications.begin(), sortedNotifications.end(),
              [](const NotificationServiceStruct& a, const NotificationServiceStruct& b) {
                  return a.timestamp > b.timestamp;
              });

    if (sortedNotifications.isEmpty()) {
        listWidget->addItem(new QListWidgetItem(NotificationsDialogConstants::kNoNotificationsText));
    } else {
        for (const auto& notif : sortedNotifications) {
            listWidget->addItem(createNotificationItem(notif));
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
