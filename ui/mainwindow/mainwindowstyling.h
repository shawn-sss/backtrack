#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

#include <QString>

namespace MainWindowStyling {

// Unified style for short feedback
inline const QString BUTTON_FEEDBACK_STYLE = QStringLiteral(
    "QPushButton {"
    "  background-color: #64B5F6;"
    "  color: black;"
    "  font-weight: bold;"
    "  border: 1px solid black;"
    "  border-radius: 5px;"
    "}"
    );

} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
