#ifndef ABOUTDIALOGSTYLING_H
#define ABOUTDIALOGSTYLING_H

// Qt includes
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QCursor>

namespace AboutDialogStyling {

inline void applyCursors(QDialogButtonBox* buttonBox) {
    const auto buttons = buttonBox->buttons();
    for (QAbstractButton* button : buttons) {
        button->setCursor(Qt::PointingHandCursor);
    }
}

} // namespace AboutDialogStyling

#endif // ABOUTDIALOGSTYLING_H
