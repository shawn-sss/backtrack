#ifndef SNAPLISTDIALOGCONSTANTS_H
#define SNAPLISTDIALOGCONSTANTS_H

namespace SnapListDialogConstants {

// Dialog dimensions
constexpr int k_MIN_WIDTH = 400;
constexpr int k_MIN_HEIGHT = 300;

// Button texts
inline constexpr const char* k_LOAD_BUTTON_TEXT = "Load";
inline constexpr const char* k_DELETE_BUTTON_TEXT = "Delete";
inline constexpr const char* k_SAVE_BUTTON_TEXT = "Save Current";

// Dialog titles and messages
inline constexpr const char* k_WINDOW_TITLE = "Manage SnapLists";
inline constexpr const char* k_DELETE_CONFIRM_TITLE = "Delete SnapList";
inline constexpr const char* k_DELETE_CONFIRM_MESSAGE = "Are you sure you want to delete '%1'?";
inline constexpr const char* k_SAVE_PROMPT_TITLE = "Save SnapList";
inline constexpr const char* k_SAVE_PROMPT_MESSAGE = "Enter name for this SnapList:";
inline constexpr const char* k_SAVE_FAILED_TITLE = "Save Failed";
inline constexpr const char* k_SAVE_FAILED_MESSAGE = "Could not save SnapList.";

// Load prompt for newly saved SnapList
inline constexpr const char* k_LOAD_NEW_PROMPT_TITLE = "Load New SnapList?";
inline constexpr const char* k_LOAD_NEW_PROMPT_MESSAGE =
    "The SnapList '%1' was saved.\n\nWould you like to load it now?\n(This will clear your current staging list)";

} // namespace SnapListDialogConstants

#endif // SNAPLISTDIALOGCONSTANTS_H
