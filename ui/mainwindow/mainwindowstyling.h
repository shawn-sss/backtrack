#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

// Qt includes
#include <QString>

namespace MainWindowStyling {

// Create backup button cooldown style
inline const QString COOLDOWN_BUTTON_STYLE = QStringLiteral(
    "QPushButton {"
    "  background-color: #FFA726;"
    "  color: black;"
    "  font-weight: bold;"
    "  border: 1px solid black;"
    "  border-radius: 5px;"
    "}"
    );

} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
