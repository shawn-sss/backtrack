#ifndef SETTINGSDIALOGCONSTANTS_H
#define SETTINGSDIALOGCONSTANTS_H

// C++ includes
#include <QString>

namespace SettingsDialogConstants {

inline constexpr const char* k_CATEGORY_USER_PREFERENCES = "User Preferences";
inline constexpr const char* k_CATEGORY_ADVANCED         = "Advanced Settings";

inline constexpr const char* k_LABEL_BACKUP_PREFIX       = "Backup Prefix:";
inline constexpr const char* k_LABEL_CLOSE_BEHAVIOR      = "Close Behavior:";
inline constexpr const char* k_LABEL_CLOSE_BEHAVIOR_DESC = "Choose what happens when you close the app window.";
inline constexpr const char* k_LABEL_MINIMIZE_ON_CLOSE   = "Minimize to system tray instead of exiting";
inline constexpr const char* k_LABEL_THEME               = "Theme:";
inline constexpr const char* k_LABEL_THEME_SYSTEM_DEFAULT= "System Default";
inline constexpr const char* k_LABEL_THEME_LIGHT_MODE    = "Light Mode";
inline constexpr const char* k_LABEL_THEME_DARK_MODE     = "Dark Mode";

inline const QString k_DESC_BACKUP_SUBTITLE =
    QStringLiteral("This is the first part of each backup name to help group and identify them.");
inline const QString k_DESC_BACKUP_INFO =
    QStringLiteral("Allowed: letters (A–Z, a–z) and digits (0–9), up to 12 characters.");

inline constexpr const char* k_BUTTON_SAVE_TEXT        = "Save";
inline constexpr const char* k_BUTTON_SAVED_TEXT       = "✔️ Saved";
inline constexpr const char* k_BUTTON_SAVE_WIDTH_TEXT  = "✔️ Saved";
inline constexpr const char* k_BUTTON_CLEAR_APP        = "🗑️ Clear App Data";
inline constexpr const char* k_BUTTON_RESET_BACKUP     = "📁 Reset Backup Archive";

inline constexpr const char* k_TOOLTIP_SAVE_BUTTON  = "Save your settings and apply changes";
inline constexpr const char* k_TOOLTIP_RESET_BACKUP = "Delete all backups and logs from the backup directory";
inline constexpr const char* k_TOOLTIP_CLEAR_APP    = "Remove all data created by this app from your system";

inline constexpr const char* k_WARNING_INVALID_PREFIX_TITLE   = "Invalid Prefix";
inline constexpr const char* k_WARNING_INVALID_PREFIX_MESSAGE = "Only letters (A–Z, a–z) and digits (0–9) are allowed.";

inline const QString k_CONFIRM_RESET_TITLE   = QStringLiteral("Delete Backup Archive");
inline const QString k_CONFIRM_RESET_MESSAGE = QStringLiteral("Are you sure you want to delete all backups in:\n%1");
inline const QString k_WARNING_INVALID_PATH_TITLE   = QStringLiteral("Invalid Location");
inline const QString k_WARNING_INVALID_PATH_MESSAGE = QStringLiteral("Backup location is invalid or does not exist.");

inline const QString k_BACKUP_PREFIX_REGEX_PATTERN = QStringLiteral("^[A-Za-z0-9]{1,12}$");

inline constexpr int k_SAVE_FEEDBACK_COOLDOWN_MS = 3000;
inline constexpr int k_DIALOG_MIN_WIDTH    = 600;
inline constexpr int k_DIALOG_MIN_HEIGHT   = 400;
inline constexpr int k_MAIN_MARGIN         = 10;
inline constexpr int k_MAIN_SPACING        = 10;

} // namespace SettingsDialogConstants

#endif // SETTINGSDIALOGCONSTANTS_H
