#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// Qt includes
#include <QDir>
#include <QSize>

// Application metadata
namespace AppInfo {
// Core app identity
constexpr auto k_APP_AUTHOR_NAME = "Shawn SSS";
constexpr auto k_APP_NAME        = "MyDataBackupApp";
constexpr auto k_APP_VERSION     = "0.5";
}

// Window configuration
namespace AppConfig {
// App window sizes
constexpr QSize k_MINIMUM_WINDOW_SIZE(500, 250);
constexpr QSize k_DEFAULT_WINDOW_SIZE(1000, 560);
constexpr QSize k_MAXIMUM_WINDOW_SIZE(2000, 800);

// Config file paths
constexpr auto k_APPDATA_SETUP_FOLDER             = "app_config";
constexpr auto k_USERDATA_SETUP_FOLDER            = "user_config";
constexpr auto k_APPDATA_SETUP_INFO_FILE          = "app_init.json";
constexpr auto k_APPDATA_SETUP_USER_SETTINGS_FILE = "user_settings.json";
}

// Backup default paths
namespace Backup::Paths {
// Default root and dialog paths
inline auto k_DEFAULT_ROOT_PATH          = QLatin1String("");
const QString k_DEFAULT_FILE_DIALOG_ROOT = QDir::rootPath();
}

// File system filters
namespace Backup::FileSystem {
// Directory and file filter
inline QDir::Filters k_FILE_SYSTEM_FILTER = QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files;
}

// Backup drive label configuration
namespace Backup::DriveConfig {
// Drive label defaults
constexpr auto k_DEFAULT_DRIVE_LABEL = "Local Disk";
constexpr auto k_DRIVE_LABEL_SUFFIX  = "Drive";
}

// Timestamp format strings
namespace Backup::Timestamps {
// Formats for folder names and UI
constexpr auto k_BACKUP_FOLDER_TIMESTAMP_FORMAT  = "_yyyyMMdd_HHmmss";
constexpr auto k_BACKUP_TIMESTAMP_DISPLAY_FORMAT = "MM/dd/yyyy hh:mm AP";
}

// Metadata keys for backup tracking
namespace Backup::MetadataKeys {
// JSON keys for backup records
constexpr auto k_NAME                     = "backup_name";
constexpr auto k_TIMESTAMP                = "backup_timestamp";
constexpr auto k_DURATION                 = "backup_duration";
constexpr auto k_DURATION_READABLE        = "backup_duration_readable";
constexpr auto k_SIZE_BYTES               = "total_size_bytes";
constexpr auto k_SIZE_READABLE            = "total_size_readable";
constexpr auto k_FILE_COUNT               = "file_count";
constexpr auto k_FOLDER_COUNT             = "folder_count";
constexpr auto k_USER_SELECTED_ITEMS      = "user_selected_items";
constexpr auto k_USER_SELECTED_ITEM_COUNT = "user_selected_item_count";
constexpr auto k_BACKUP_FILES             = "backup_files";
constexpr auto k_BACKUP_FOLDERS           = "backup_folders";
}

// Backup system infrastructure paths
namespace Backup::Infrastructure {
// Folders and logs
constexpr auto k_BACKUP_SETUP_FOLDER  = "_MyDataBackupApp";
constexpr auto k_BACKUP_SETUP_INFO_FILE = "backup_init.json";
constexpr auto k_BACKUP_LOGS_FOLDER     = "logs";
inline auto k_BACKUP_LOGS_FILE          = QStringLiteral("log.json");
}

// Application icon
namespace Resources::Application {
// Icon path
constexpr auto k_ICON_PATH = ":/resources/app_icon.png";
}

// Timing values
namespace Timing {
// UI timing settings
constexpr int k_BUTTON_FEEDBACK_DURATION_MS = 1000;
}

// Fallback utility values
namespace Utilities {
// Default fallback strings
constexpr auto k_DEFAULT_VALUE_NOT_AVAILABLE = "N/A";
}

// File size unit strings and conversion factor
namespace Units::FileSize {
// File size units and conversion
constexpr auto k_SIZE_UNIT_BYTES       = " B";
constexpr auto k_SIZE_UNIT_KILOBYTES   = " KB";
constexpr auto k_SIZE_UNIT_MEGABYTES   = " MB";
constexpr auto k_SIZE_UNIT_GIGABYTES   = " GB";
constexpr double k_SIZE_CONVERSION_FACTOR = 1024.0;
}

// Time unit strings
namespace Units::Time {
// Time duration units
constexpr auto k_UNIT_MILLISECONDS = " milliseconds";
constexpr auto k_UNIT_SECONDS      = " seconds";
constexpr auto k_UNIT_MINUTES      = " minutes";
constexpr auto k_UNIT_HOURS        = " hours";
constexpr auto k_UNIT_DAYS         = " days";
}

// Tree view UI settings
namespace UISettings::TreeView {
// Tree column setup
constexpr auto k_STAGING_COLUMN_NAME = "Name";
constexpr int k_START_HIDDEN_COLUMN  = 1;
constexpr int k_DEFAULT_COLUMN_COUNT = 4;
}

// Progress message and logic settings
namespace ProgressSettings {
// Progress bar logic and display
constexpr auto k_PROGRESS_BAR_INITIAL_MESSAGE    = "Waiting for backup activity...";
constexpr auto k_PROGRESS_BAR_COMPLETION_MESSAGE = "Operation has completed!";
constexpr bool k_PROGRESS_BAR_DEFAULT_VISIBILITY = false;
constexpr bool k_PROGRESS_BAR_TEXT_VISIBLE       = true;
constexpr int k_PROGRESS_BAR_MIN_VALUE           = 0;
constexpr int k_PROGRESS_BAR_MAX_VALUE           = 100;
constexpr int k_PROGRESS_BAR_DEFAULT_VALUE       = k_PROGRESS_BAR_MIN_VALUE;
constexpr int k_PROGRESS_BAR_DELAY_MS            = 1000;
}

// Progress UI layout values
namespace ProgressUI {
// Progress display sizes
constexpr int k_STATUS_LIGHT_SIZE   = 8;
constexpr int k_PROGRESS_BAR_HEIGHT = 20;
}

#endif // APP_CONFIG_H
