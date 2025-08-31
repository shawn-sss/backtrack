#ifndef EXPORTSERVICECONSTANTS_H
#define EXPORTSERVICECONSTANTS_H

// Project includes
#include "../../../constants/app_info.h"

// C++ includes
#include <QString>

namespace ExportServiceConstants {

// Category
inline constexpr const char* k_CATEGORY_EXPORT = "Export";

// Buttons
inline constexpr const char* k_BUTTON_EXPORT_PREFS = "📤 Export Preferences";

// Tooltips
inline constexpr const char* k_TOOLTIP_EXPORT_PREFS = "Export your user settings and templates into a single backup file.";

// File handling
inline constexpr const char* k_EXPORT_DEFAULT_FILENAME = App::Items::k_APPDATA_COMBINED_PREFERENCES_FILE;
inline constexpr const char* k_EXPORT_FILE_FILTER      = "JSON Files (*.json);;All Files (*)";

// JSON keys
inline constexpr const char* k_JSON_KEY_USER_SETTINGS  = "user_settings";
inline constexpr const char* k_JSON_KEY_USER_TEMPLATES = "user_templates";

// Dialog messages
inline constexpr const char* k_EXPORT_SUCCESS_TITLE   = "Export Successful";
inline constexpr const char* k_EXPORT_SUCCESS_MESSAGE = "Your user preferences were successfully exported.";
inline constexpr const char* k_EXPORT_FAILURE_TITLE   = "Export Failed";
inline constexpr const char* k_EXPORT_FAILURE_MESSAGE = "An error occurred while exporting preferences.";
inline constexpr const char* k_EXPORT_FAILURE_GENERIC = "Failed to write export file.";

} // namespace ExportServiceConstants

#endif
