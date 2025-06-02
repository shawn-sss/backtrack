#ifndef SETTINGSDIALOGCONSTANTS_H
#define SETTINGSDIALOGCONSTANTS_H

// Qt includes
#include <QString>

namespace SettingsDialogConstants {

// Settings Window button tooltips
inline constexpr auto k_RESET_DESTINATION  = "Delete ALL contents from the backup destination";
inline constexpr auto k_UNINSTALL          = "Uninstall the application and delete all local data";

// Labels
inline constexpr auto k_LABEL_BACKUP_PREFIX           = "Backup Prefix:";
inline constexpr auto k_LABEL_BACKUP_PREFIX_TOOLTIP   = "Modify the prefix used when creating backups.";
inline constexpr auto k_LABEL_THEME                   = "Theme:";
inline constexpr auto k_LABEL_THEME_SYSTEM_DEFAULT    = "System Default";
inline constexpr auto k_LABEL_THEME_LIGHT_MODE        = "Light Mode";
inline constexpr auto k_LABEL_THEME_DARK_MODE         = "Dark Mode";

// Descriptions
inline constexpr auto k_DESC_BACKUP_PREFIX            = "Backup Prefix Description:";
inline constexpr auto k_DESC_BACKUP_SUBTITLE          = "This is the first part of each backup name to help group and identify them.";
inline constexpr auto k_DESC_BACKUP_INFO              = "Allowed: letters (A–Z, a–z) and digits (0–9), up to 12 characters.";

// Button labels
inline constexpr auto k_BUTTON_SAVE_TEXT              = "Save";
inline constexpr auto k_BUTTON_SAVED_TEXT             = "✔️ Saved";
inline constexpr auto k_BUTTON_SAVE_WIDTH_TEXT        = "✔️ Saved";
inline constexpr auto k_BUTTON_CLEAR_APP              = "🗑️ Clear App Data";
inline constexpr auto k_BUTTON_RESET_BACKUP           = "📁 Reset Backup Archive";

// Tooltips
inline constexpr auto k_TOOLTIP_SAVE_BUTTON           = "Save your settings and apply changes";
inline constexpr auto k_TOOLTIP_RESET_BACKUP          = "Delete all backups and logs from the backup directory";
inline constexpr auto k_TOOLTIP_CLEAR_APP             = "Remove all data created by this app from your system";

// Input validation warning
inline constexpr auto k_WARNING_INVALID_PREFIX_TITLE   = "Invalid Prefix";
inline constexpr auto k_WARNING_INVALID_PREFIX_MESSAGE = "Only letters (A–Z, a–z) and digits (0–9) are allowed.";

// Dialog titles and messages
inline const QString k_CONFIRM_RESET_TITLE            = QStringLiteral("Delete Backup Archive");
inline const QString k_CONFIRM_RESET_MESSAGE          = QStringLiteral("Are you sure you want to delete all backups in:\n%1");

inline constexpr auto k_RESET_SUCCESS_TITLE           = "Reset Complete";
inline constexpr auto k_RESET_SUCCESS_MESSAGE         = "All backup data and logs have been deleted.";

inline constexpr auto k_RESET_FAILURE_TITLE           = "Reset Failed";
inline constexpr auto k_RESET_FAILURE_MESSAGE         = "Some items could not be deleted.";

inline constexpr auto k_WARNING_INVALID_PATH_TITLE    = "Invalid Location";
inline constexpr auto k_WARNING_INVALID_PATH_MESSAGE  = "Backup location is invalid or does not exist.";

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
