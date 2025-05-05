#ifndef SETTINGSDIALOGSTYLING_H
#define SETTINGSDIALOGSTYLING_H

// Qt includes
#include <QString>

namespace SettingsDialogStyling {

// Special case style for disabled/cooldown Save button
inline const QString COOLDOWN_BUTTON_STYLE = QStringLiteral(
    "QPushButton#CooldownButton {"
    "  background-color: #cccccc;"
    "  color: #333;"
    "  font-weight: bold;"
    "  border: 1px solid #999;"
    "}");

// Shared QPushButton style is applied globally

} // namespace SettingsDialogStyling

#endif // SETTINGSDIALOGSTYLING_H
