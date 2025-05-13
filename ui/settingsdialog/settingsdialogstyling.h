#ifndef SETTINGSDIALOGSTYLING_H
#define SETTINGSDIALOGSTYLING_H

// Qt includes
#include <QString>

namespace SettingsDialogStyling {

// Style for Save button when in cooldown state
inline constexpr auto COOLDOWN_BUTTON_STYLE = R"(
    QPushButton#CooldownButton {
        background-color: #cccccc;
        color: #333;
        font-weight: bold;
        border: 1px solid #999;
    }
)";

} // namespace SettingsDialogStyling

#endif // SETTINGSDIALOGSTYLING_H
