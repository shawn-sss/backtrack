#ifndef SETTINGSDIALOGCONSTANTS_H
#define SETTINGSDIALOGCONSTANTS_H

// Qt includes
#include <QString>

namespace SettingsDialogConstants {

// Labels
inline constexpr auto k_LABEL_BACKUP_PREFIX           = "Backup Prefix:";
inline constexpr auto k_LABEL_BACKUP_PREFIX_TOOLTIP   = "Modify the prefix used when creating backups.";
inline constexpr auto k_LABEL_THEME                   = "Theme:";
inline constexpr auto k_LABEL_THEME_SYSTEM_DEFAULT    = "System Default";
inline constexpr auto k_LABEL_THEME_LIGHT_MODE        = "Light Mode";
inline constexpr auto k_LABEL_THEME_DARK_MODE         = "Dark Mode";

// Save button
inline constexpr auto k_BUTTON_SAVE_TEXT              = "Save";
inline constexpr auto k_BUTTON_SAVED_TEXT             = "✔️ Saved";
inline constexpr auto k_BUTTON_SAVE_WIDTH_TEXT        = "✔️ Saved";

// Timing
inline constexpr int k_SAVE_FEEDBACK_COOLDOWN_MS      = 3000;

// Layout
inline constexpr int k_CATEGORY_LIST_WIDTH            = 150;
inline constexpr int k_DIALOG_MIN_WIDTH               = 600;
inline constexpr int k_DIALOG_MIN_HEIGHT              = 400;
inline constexpr int k_MAIN_MARGIN                    = 10;
inline constexpr int k_MAIN_SPACING                   = 10;

} // namespace SettingsDialogConstants

#endif // SETTINGSDIALOGCONSTANTS_H
