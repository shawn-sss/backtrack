#ifndef SETTINGSDIALOGSTYLING_H
#define SETTINGSDIALOGSTYLING_H

// Qt includes
#include <QString>

namespace SettingsDialogStyling {

// Save button cooldown style
inline const QString COOLDOWN_BUTTON_STYLE = QStringLiteral(
    "QPushButton {"
    "  background-color: #4CAF50;"
    "  color: white;"
    "  font-weight: bold;"
    "}"
    );

} // namespace SettingsDialogStyling

#endif // SETTINGSDIALOGSTYLING_H
