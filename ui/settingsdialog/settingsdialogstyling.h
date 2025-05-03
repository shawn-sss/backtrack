#ifndef SETTINGSDIALOGSTYLING_H
#define SETTINGSDIALOGSTYLING_H

// Qt includes
#include <QString>

namespace SettingsDialogStyling {

// Style applied to the Save button during cooldown feedback
inline const QString COOLDOWN_BUTTON_STYLE = QStringLiteral(
    "QPushButton {"
    "  background-color: #28a745;"
    "  color: black;"
    "  font-weight: bold;"
    "  border: 1px solid black;"
    "  border-radius: 5px;"
    "}"
    );

} // namespace SettingsDialogStyling

#endif // SETTINGSDIALOGSTYLING_H
