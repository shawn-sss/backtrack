#ifndef TEMPLATEDIALOGCONSTANTS_H
#define TEMPLATEDIALOGCONSTANTS_H

namespace TemplateDialogConstants {

inline constexpr int k_MIN_WIDTH  = 700;
inline constexpr int k_MIN_HEIGHT = 400;

inline constexpr int k_ACTION_BUTTON_SIZE    = 24;
inline constexpr int k_ACTION_LAYOUT_MARGIN  = 0;
inline constexpr int k_ACTION_LAYOUT_SPACING = 2;

inline constexpr const char* k_WINDOW_TITLE = "Manage Templates";

inline constexpr const char* k_HEADER_NAME     = "Template";
inline constexpr const char* k_HEADER_ITEMS    = "Items";
inline constexpr const char* k_HEADER_MODIFIED = "Last Modified";
inline constexpr const char* k_HEADER_ACTIONS  = "Actions";

inline constexpr const char* k_NEW_TEMPLATE_BUTTON_TEXT = "➕ New Template";
inline constexpr const char* k_CLOSE_BUTTON_TEXT        = "Close";

inline constexpr const char* k_TOOLTIP_LOAD    = "Load this template";
inline constexpr const char* k_TOOLTIP_UNLOAD  = "Unload this template";
inline constexpr const char* k_TOOLTIP_DELETE  = "Delete this template";
inline constexpr const char* k_TOOLTIP_DEFAULT = "Set or unset as default";
inline constexpr const char* k_TOOLTIP_NEW     = "Save current staging as a new template";

inline constexpr const char* k_DELETE_CONFIRM_TITLE   = "Delete Template";
inline constexpr const char* k_DELETE_CONFIRM_MESSAGE = "Are you sure you want to delete '%1'?";

inline constexpr const char* k_INVALID_NAME_TITLE   = "Invalid Template Name";
inline constexpr const char* k_INVALID_NAME_MESSAGE = "Template name cannot be empty.";

inline constexpr const char* k_DUPLICATE_NAME_TITLE   = "Duplicate Template Name";
inline constexpr const char* k_DUPLICATE_NAME_MESSAGE = "A template named '%1' already exists.";

inline constexpr const char* k_SAVE_FAILED_TITLE      = "Save Failed";
inline constexpr const char* k_SAVE_FAILED_MESSAGE    = "Could not save template.";
inline constexpr const char* k_EMPTY_TEMPLATE_MESSAGE = "Cannot save an empty template.";

inline constexpr const char* k_RENAME_PROMPT_TITLE   = "Rename Template";
inline constexpr const char* k_RENAME_PROMPT_MESSAGE = "Enter a new name for the template:";
inline constexpr const char* k_NEW_TEMPLATE_DIALOG_TITLE = "Create New Template";

inline constexpr const char* k_DEFAULT_SET_MESSAGE   = "'%1' is now the default.";
inline constexpr const char* k_DEFAULT_UNSET_MESSAGE = "'%1' is no longer the default.";

inline constexpr const char* k_DEFAULT_MARKER = "⭐";
inline constexpr const char* k_DEFAULT_SUFFIX = " (Default)";

inline constexpr const char* k_SEARCH_PLACEHOLDER = "Search templates...";

inline constexpr const char* k_ICON_LOAD    = "⏎";
inline constexpr const char* k_ICON_UNLOAD  = "⏏";
inline constexpr const char* k_ICON_DELETE  = "🗑";
inline constexpr const char* k_ICON_DEFAULT = "★";

inline constexpr const char* k_PREVIEW_LABEL = "Template Contents:";
inline constexpr const char* k_DATE_FORMAT   = "yyyy-MM-dd hh:mm:ss";
inline constexpr const char* k_FOLDER_SUFFIX = "/";

} // namespace TemplateDialogConstants

#endif // TEMPLATEDIALOGCONSTANTS_H
