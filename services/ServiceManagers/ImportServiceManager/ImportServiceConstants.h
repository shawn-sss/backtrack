#ifndef IMPORTSERVICECONSTANTS_H
#define IMPORTSERVICECONSTANTS_H

// C++ includes
#include <QString>

namespace ImportServiceConstants {

// Buttons
inline constexpr const char* k_BUTTON_IMPORT_PREFS = "📥 Import Preferences";

// Tooltips
inline constexpr const char* k_TOOLTIP_IMPORT_PREFS =
    "Import user settings and templates from a backup file.";

// File handling
inline constexpr const char* k_IMPORT_FILE_FILTER = "JSON Files (*.json);;All Files (*)";

// JSON keys
inline constexpr const char* k_JSON_KEY_USER_SETTINGS  = "user_settings";
inline constexpr const char* k_JSON_KEY_USER_TEMPLATES = "user_templates";

// Dialog messages
inline constexpr const char* k_IMPORT_SUCCESS_TITLE   = "Import Successful";
inline constexpr const char* k_IMPORT_SUCCESS_MESSAGE = "Your user preferences were successfully imported.";
inline constexpr const char* k_IMPORT_FAILURE_TITLE   = "Import Failed";
inline constexpr const char* k_IMPORT_FAILURE_MESSAGE = "An error occurred while importing preferences.";
inline constexpr const char* k_IMPORT_INVALID_FILE_TITLE   = "Invalid File";
inline constexpr const char* k_IMPORT_INVALID_FILE_MESSAGE = "The selected file is not a valid preferences backup.";

// Error details
inline constexpr const char* k_IMPORT_ERROR_EMPTY_FILE   = "Invalid or empty file.";
inline constexpr const char* k_IMPORT_ERROR_MISSING_KEYS = "File is missing required data.";
inline constexpr const char* k_IMPORT_ERROR_WRITE_FAIL   = "Failed to write imported preferences.";

} // namespace ImportServiceConstants

#endif
