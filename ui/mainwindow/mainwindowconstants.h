#ifndef MAINWINDOWCONSTANTS_H
#define MAINWINDOWCONSTANTS_H

// C++ includes
#include <QString>

namespace MainWindowConstants {

// Tooltips
inline constexpr auto k_ADD_TO_BACKUP      = "Add selected items to staging for backup";
inline constexpr auto k_REMOVE_FROM_BACKUP = "Remove selected items from staging";
inline constexpr auto k_CREATE_BACKUP      = "Create a new backup with staged items";
inline constexpr auto k_CHANGE_DESTINATION = "Change the backup destination folder";
inline constexpr auto k_DELETE_BACKUP      = "Delete the selected backup";
inline constexpr auto k_NOTIFICATIONS      = "View application notifications";
inline constexpr auto k_UNLOCK_DRIVE       = "Unlock an encrypted drive";
inline constexpr auto k_TEMPLATE           = "Load a backup template";
inline constexpr auto k_SCHEDULE           = "Schedule automated backups";

// Tray actions
inline constexpr auto k_TRAY_ACTION_OPEN     = "Open";
inline constexpr auto k_TRAY_ACTION_SETTINGS = "Settings";
inline constexpr auto k_TRAY_ACTION_EXIT     = "Exit";

// BitLocker unlock integration
inline constexpr auto k_TASKKILL_CMD        = "taskkill";
inline constexpr auto k_TASKKILL_ARG_IM     = "/IM";
inline constexpr auto k_TASKKILL_ARG_FORCE  = "/F";

inline constexpr auto k_MANAGE_BDE_EXE      = "manage-bde.exe";

// Unlock arguments
inline constexpr auto k_UNLOCK_ARGS_PASSWORD = "-unlock %1: -password";

// PowerShell helpers
inline constexpr auto k_POWERSHELL_CMD      = "powershell.exe";
inline constexpr auto k_PS_START_PROCESS    = "Start-Process";
inline constexpr auto k_PS_ARG_ARGUMENTLIST = "-ArgumentList";
inline constexpr auto k_PS_ARG_VERB_RUNAS   = "-Verb runAs";

// PowerShell script, %1 = exe, %2 = drive letter, %3 = unlock args
inline constexpr auto k_UNLOCK_SCRIPT_TEMPLATE =
    "Start-Process %1 -ArgumentList '%3' -Verb runAs";

// Timing and limits
inline constexpr int k_BACKUP_LOCATION_MAXCHARS = 50;
inline constexpr int k_SCHEDULE_FIRE_DELAY_MS   = 1000;
inline constexpr int k_DRIVE_UNLOCK_DELAY_MS    = 5000;

} // namespace MainWindowConstants

#endif // MAINWINDOWCONSTANTS_H
