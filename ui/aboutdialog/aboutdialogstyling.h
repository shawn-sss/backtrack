#ifndef ABOUTDIALOGSTYLING_H
#define ABOUTDIALOGSTYLING_H

#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QCursor>
#include <QLabel>
#include <QString>
#include "../../../../constants/app_info.h"

namespace AboutDialogStyling {

inline void applyCursors(QDialogButtonBox* buttonBox) {
    for (QAbstractButton* button : buttonBox->buttons()) {
        button->setCursor(Qt::PointingHandCursor);
    }
}

inline QString buildAboutHtmlText() {
    return QString("<h2>%1</h2>"
                   "<p>Version: <b>%2</b></p>"
                   "<p>Author: <b>%3</b></p>"
                   "<p style='margin-top:%4px;'>A simple, efficient backup utility focused on transparency, control, and clarity.</p>")
        .arg(App::Info::k_APP_NAME,
             App::Info::k_APP_VERSION,
             App::Info::k_AUTHOR_NAME)
        .arg(AboutDialogConstants::kTopMargin);
}

inline void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    if (richText)
        label->setTextFormat(Qt::RichText);
}

} // namespace AboutDialogStyling

#endif // ABOUTDIALOGSTYLING_H
