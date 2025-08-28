// filename: notificationsdialog.cpp

// Project includes
#include "notificationsdialog.h"
#include "NotificationsDialogStyling.h"
#include "NotificationsDialogConstants.h"
#include "../promptdialog/promptdialog.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"

// Qt includes
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QAbstractItemView>
#include <QAbstractScrollArea>
#include <QLocale>
#include <QBrush>
#include <QDateTime>

// C++ includes
#include <algorithm>

namespace {

QString formatNotificationText(const NotificationServiceStruct& notif) {
    const QString dateStr =
        QLocale().toString(notif.timestamp.toLocalTime(), QStringLiteral("MMM d, yyyy - h:mm AP"));
    const QString badge = notif.read ? QString() : QStringLiteral("🔴 ");
    return QStringLiteral("%1%2\n%3").arg(badge, dateStr, notif.message);
}

QListWidgetItem* createNotificationItem(const NotificationServiceStruct& notif) {
    auto* item = new QListWidgetItem(formatNotificationText(notif));
    if (notif.read) {
        item->setForeground(QBrush(Qt::gray));
    } else {
        QFont f = item->font();
        f.setBold(true);
        item->setFont(f);
    }
    return item;
}

} // namespace

// Constructs NotificationsDialog and initializes UI, list, and actions
NotificationsDialog::NotificationsDialog(const QList<NotificationServiceStruct>& notifications, QWidget* parent)
    : QDialog(parent) {

    setWindowTitle(NotificationsDialogConstants::kWindowTitle);
    resize(NotificationsDialogConstants::kDialogWidth, NotificationsDialogConstants::kDialogHeight);

    auto* layout = new QVBoxLayout(this);

    listWidget = new QListWidget(this);
    listWidget->setWordWrap(true);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    listWidget->setAlternatingRowColors(true);
    listWidget->setStyleSheet(NotificationsDialogStyling::kListWidgetStyle);

    clearAllButton = new QPushButton(NotificationsDialogConstants::kClearAllText, this);
    closeButton    = new QPushButton(NotificationsDialogConstants::kCloseText, this);

    Shared::UI::applyButtonTooltipAndCursor(clearAllButton, NotificationsDialogConstants::kClearAllTooltip);
    Shared::UI::applyButtonTooltipAndCursor(closeButton,    NotificationsDialogConstants::kCloseTooltip);

    QList<NotificationServiceStruct> sortedNotifications = notifications;
    std::sort(sortedNotifications.begin(), sortedNotifications.end(),
              [](const NotificationServiceStruct& a, const NotificationServiceStruct& b) {
                  return a.timestamp > b.timestamp;
              });

    if (sortedNotifications.isEmpty()) {
        listWidget->addItem(new QListWidgetItem(NotificationsDialogConstants::kNoNotificationsText));
    } else {
        for (const NotificationServiceStruct& notif : sortedNotifications) {
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
        const auto reply = PromptDialog::showDialog(
            this,
            PromptDialog::Question,
            NotificationsDialogConstants::kConfirmTitle,
            NotificationsDialogConstants::kConfirmMessage,
            QString(),
            PromptDialog::Yes | PromptDialog::Cancel,
            PromptDialog::Cancel);

        if (reply == PromptDialog::Yes) {
            NotificationServiceManager::instance().clearAllNotifications();

            PromptDialog::showDialog(
                this,
                PromptDialog::Information,
                NotificationsDialogConstants::kClearedTitle,
                NotificationsDialogConstants::kClearedMessage,
                QString(),
                PromptDialog::Ok,
                PromptDialog::Ok);

            accept();
        }
    });
}
