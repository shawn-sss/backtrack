#ifndef HELPDIALOGSTYLING_H
#define HELPDIALOGSTYLING_H

// Qt includes
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QTabBar>
#include <QTabWidget>

namespace HelpDialogStyling {

inline void applyCursors(QTabWidget* tabWidget, QDialogButtonBox* buttonBox) {
    if (QTabBar* tabBar = tabWidget->findChild<QTabBar*>()) {
        tabBar->setCursor(Qt::PointingHandCursor);
    }

    const auto buttons = buttonBox->buttons();
    for (QAbstractButton* button : buttons) {
        button->setCursor(Qt::PointingHandCursor);
    }
}

} // namespace HelpDialogStyling

#endif // HELPDIALOGSTYLING_H
