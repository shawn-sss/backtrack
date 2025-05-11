#ifndef SETTINGSDIALOGSTYLING_H
#define SETTINGSDIALOGSTYLING_H

// Qt includes
#include <QString>

namespace SettingsDialogStyling {

// Style for Save button when disabled (cooldown state)
inline const QString COOLDOWN_BUTTON_STYLE = QStringLiteral(
    "QPushButton#CooldownButton {"
    "  background-color: #cccccc;"
    "  color: #333;"
    "  font-weight: bold;"
    "  border: 1px solid #999;"
    "}");

} // namespace SettingsDialogStyling

#endif // SETTINGSDIALOGSTYLING_H
