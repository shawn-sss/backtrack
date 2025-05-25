#ifndef SETTINGSDIALOGSTYLING_H
#define SETTINGSDIALOGSTYLING_H

// Qt includes
#include <QLabel>
#include <QFont>

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

// Style for the Clear App Data button
inline constexpr auto k_CLEAR_APP_BUTTON_STYLE = R"(
    background-color: #e74c3c;
    color: white;
    border: none;
    padding: 10px;
    border-radius: 6px;
    font-weight: bold;
)";

// Style for the Reset Backup Archive button
inline constexpr auto k_RESET_BACKUP_BUTTON_STYLE = R"(
    background-color: #f39c12;
    color: white;
    border: none;
    padding: 10px;
    border-radius: 6px;
    font-weight: bold;
)";

} // namespace SettingsDialogStyling

#endif // SETTINGSDIALOGSTYLING_H
