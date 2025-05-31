#ifndef UIUTILSSERVICECONSTANTS_H
#define UIUTILSSERVICECONSTANTS_H

// Qt includes
#include <QString>

namespace UIUtilsServiceConstants {
namespace ToolTips {

// Tooltips for UI actions
inline constexpr auto k_ADD_TO_BACKUP      = "Add selected items to the backup staging area";
inline constexpr auto k_REMOVE_FROM_BACKUP = "Remove selected items from the backup staging area";
inline constexpr auto k_CREATE_BACKUP      = "Start the backup process";
inline constexpr auto k_CHANGE_DESTINATION = "Change the destination folder for backups";
inline constexpr auto k_DELETE_BACKUP      = "Delete the selected backup from the destination view";
inline constexpr auto k_RESET_DESTINATION  = "Delete ALL contents from the backup destination";
inline constexpr auto k_NOTIFICATIONS      = "View backup notifications";
inline constexpr auto k_UNINSTALL          = "Uninstall the application and delete all local data";
inline constexpr auto k_UNLOCK_DRIVE       = "Prompt to unlock a BitLocker-protected drive";
inline constexpr auto k_SNAP_LIST          = "Manage SnapLists";
inline constexpr auto k_RESET_SNAP_LIST    = "Unload SnapList";

} // namespace ToolTips
} // namespace UIUtilsServiceConstants

#endif // UIUTILSSERVICECONSTANTS_H
