#ifndef ABOUTDIALOGSTYLING_H
#define ABOUTDIALOGSTYLING_H

// Project includes
#include "../../../../constants/app_info.h"
#include "aboutdialogconstants.h"

// Qt includes
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QCursor>
#include <QLabel>
#include <QString>

// Styling utilities for the About dialog
namespace AboutDialogStyling {

// Applies pointing hand cursor to dialog buttons
inline void applyCursors(QDialogButtonBox* buttonBox) {
    const QList<QAbstractButton*>& buttons = buttonBox->buttons();
    for (QAbstractButton* button : buttons)
        button->setCursor(Qt::PointingHandCursor);
}

// Builds the HTML content for the About dialog text
inline QString buildAboutHtmlText() {
    return QString(
               "<h2>%1</h2>"
               "<p>Version: <b>%2</b></p>"
               "<p>Author: <b>%3</b></p>"
               "<p style='margin-top:%4px;'>A simple, efficient backup utility focused on transparency, control, and clarity.</p>")
        .arg(App::Info::k_APP_NAME,
             App::Info::k_APP_VERSION,
             App::Info::k_AUTHOR_NAME)
        .arg(AboutDialogConstants::kTopMargin);
}

// Styles a QLabel for centered, wrapped rich/plain text
inline void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    if (richText)
        label->setTextFormat(Qt::RichText);
}

}

#endif // ABOUTDIALOGSTYLING_H
