#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

// Qt includes
#include <QString>

namespace MainWindowStyling {

// Create backup button cooldown style
inline const QString COOLDOWN_BUTTON_STYLE = QStringLiteral(
    "QPushButton {"
    "  background-color: #4CAF50;"
    "  color: white;"
    "  font-weight: bold;"
    "}"
    );

} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
