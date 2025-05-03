#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

// Qt includes
#include <QString>

namespace SettingsDialogConfig {

// UI labels and tooltips
constexpr auto k_LABEL_BACKUP_PREFIX = "Backup Prefix:";
constexpr auto k_LABEL_BACKUP_PREFIX_TOOLTIP = "Modify the prefix used when creating backups.";
constexpr auto k_LABEL_THEME = "Theme:";
constexpr auto k_LABEL_THEME_SYSTEM_DEFAULT = "System Default";
constexpr auto k_LABEL_THEME_LIGHT_MODE = "Light Mode";
constexpr auto k_LABEL_THEME_DARK_MODE = "Dark Mode";
constexpr auto k_BUTTON_SAVE_TEXT = "Save";
constexpr auto k_BUTTON_SAVED_TEXT = "✔️ Saved";
constexpr auto k_BUTTON_SAVE_WIDTH_TEXT = "🟢 Saved";

// Timing for save feedback display
constexpr int k_SAVE_FEEDBACK_COOLDOWN_MS = 3000;

// Dialog layout dimensions
constexpr int k_CATEGORY_LIST_WIDTH = 150;
constexpr int k_DIALOG_MIN_WIDTH = 600;
constexpr int k_DIALOG_MIN_HEIGHT = 400;
constexpr int k_MAIN_MARGIN = 10;
constexpr int k_MAIN_SPACING = 10;

} // namespace SettingsDialogConfig

#endif // DIALOG_SETTINGS_H
