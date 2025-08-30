#ifndef MAINWINDOWCONSTANTS_H
#define MAINWINDOWCONSTANTS_H

// MainWindow button tooltips
inline constexpr auto k_UNLOCK_DRIVE        = "Prompt to unlock a BitLocker-protected drive";
inline constexpr auto k_ADD_TO_BACKUP       = "Add selected items to the backup staging area";
inline constexpr auto k_REMOVE_FROM_BACKUP  = "Remove selected items from the backup staging area";
inline constexpr auto k_CREATE_BACKUP       = "Start the backup process";
inline constexpr auto k_CHANGE_DESTINATION  = "Change the destination folder for backups";
inline constexpr auto k_DELETE_BACKUP       = "Delete the selected backup from the destination view";
inline constexpr auto k_NOTIFICATIONS       = "View backup notifications";
inline constexpr auto k_TEMPLATE            = "Manage Templates";
inline constexpr auto k_RESET_TEMPLATE      = "Unload Template";
inline constexpr auto k_SCHEDULE            = "Manage backup schedule";

// Tray menu actions
inline constexpr auto k_TRAY_ACTION_OPEN     = "Open";
inline constexpr auto k_TRAY_ACTION_SETTINGS = "Settings...";
inline constexpr auto k_TRAY_ACTION_EXIT     = "Exit";

// Drive unlock (BitLocker)
inline constexpr auto k_TASKKILL_CMD   = "taskkill";
inline constexpr auto k_MANAGE_BDE_EXE = "manage-bde.exe";
inline constexpr auto k_UNLOCK_ARGS    = "-unlock %1: -password";
inline constexpr auto k_POWERSHELL_CMD = "powershell";

// Miscellaneous constants
inline constexpr int  k_BACKUP_LOCATION_MAXCHARS = 45;
inline constexpr auto k_WRITE_TEST_FILE          = ".writeTest";

// Timing (ms)
inline constexpr int k_SCHEDULE_FIRE_DELAY_MS = 800;
inline constexpr int k_DRIVE_UNLOCK_DELAY_MS  = 5000;

#endif // MAINWINDOWCONSTANTS_H
