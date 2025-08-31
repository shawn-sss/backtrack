// Project includes
#include "notificationsdialog.h"
#include "notificationsdialogstyling.h"
#include "notificationsdialogconstants.h"
#include "../promptdialog/promptdialog.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"

// Qt includes
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QAbstractItemView>
#include <QAbstractScrollArea>
#include <QLocale>
#include <QBrush>
#include <QDateTime>
#include <QFont>

// C++ includes
#include <algorithm>

namespace {

QString formatNotificationText(const NotificationSettings::NotificationServiceStruct& notif) {
    const QString dateStr =
        QLocale().toString(notif.timestamp.toLocalTime(),
                           QStringLiteral("MMM d, yyyy - h:mm AP"));
    const QString badge = notif.read ? QString() : QStringLiteral("🔴 ");
    return QStringLiteral("%1%2\n%3").arg(badge, dateStr, notif.message);
}

QListWidgetItem* createNotificationItem(const NotificationSettings::NotificationServiceStruct& notif) {
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

// Construct notifications dialog window
NotificationsDialog::NotificationsDialog(
    const QList<NotificationSettings::NotificationServiceStruct>& notifications,
    QWidget* parent)
    : QDialog(parent)
{
    using namespace NotificationsDialogConstants;
    using namespace NotificationsDialogStyling;

    setWindowTitle(tr(k_WINDOW_TITLE));
    resize(k_DIALOG_WIDTH, k_DIALOG_HEIGHT);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    layout->setSpacing(k_MAIN_SPACING);

    listWidget = new QListWidget(this);
    listWidget->setWordWrap(true);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    listWidget->setAlternatingRowColors(true);
    listWidget->setStyleSheet(NotificationsDialogStyling::Styles::k_LIST_WIDGET_STYLE);

    clearAllButton = new QPushButton(tr(k_CLEAR_ALL_TEXT), this);
    clearAllButton->setStyleSheet(NotificationsDialogStyling::Styles::k_BUTTON_STYLE);

    QList<NotificationSettings::NotificationServiceStruct> sortedNotifications = notifications;
    std::sort(sortedNotifications.begin(), sortedNotifications.end(),
              [](const auto& a, const auto& b) { return a.timestamp > b.timestamp; });

    if (sortedNotifications.isEmpty()) {
        listWidget->addItem(new QListWidgetItem(tr(k_NO_NOTIFICATIONS_TEXT)));
    } else {
        for (const auto& notif : sortedNotifications) {
            listWidget->addItem(createNotificationItem(notif));
        }
        listWidget->setCurrentRow(0);
        listWidget->scrollToItem(listWidget->item(0), QAbstractItemView::PositionAtTop);
    }

    layout->addWidget(listWidget);
    layout->addWidget(clearAllButton, 0, Qt::AlignHCenter);

    connect(clearAllButton, &QPushButton::clicked, this, [this]() {
        using namespace NotificationsDialogConstants;

        const auto reply = PromptDialog::showDialog(
            this,
            PromptDialog::Question,
            tr(k_CONFIRM_TITLE),
            tr(k_CONFIRM_MESSAGE),
            QString(),
            PromptDialog::Yes | PromptDialog::Cancel,
            PromptDialog::Cancel);

        if (reply == PromptDialog::Yes) {
            NotificationServiceManager::instance().clearAllNotifications();

            PromptDialog::showDialog(
                this,
                PromptDialog::Information,
                tr(k_CLEARED_TITLE),
                tr(k_CLEARED_MESSAGE),
                QString(),
                PromptDialog::Ok,
                PromptDialog::Ok);

            accept();
        }
    });
}
