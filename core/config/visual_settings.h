#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QString>

namespace UIConfig {

// Backup Directory UI Labels
inline const QString LABEL_BACKUP_FOUND = "Backup Found: ";
inline const QString LABEL_BACKUP_LOCATION = "Backup Location: ";
inline const QString LABEL_BACKUP_TOTAL_COUNT = "Backup Total Count: ";
inline const QString LABEL_BACKUP_TOTAL_SIZE = "Backup Total Size: ";
inline const QString LABEL_BACKUP_LOCATION_ACCESS = "Backup Location Access: ";

// Directory Write Status
inline const QString DIRECTORY_STATUS_WRITABLE = "Writable";
inline const QString DIRECTORY_STATUS_READ_ONLY = "Read-Only";
inline const QString DIRECTORY_STATUS_UNKNOWN = "Unknown";

// Last Backup UI Labels
inline const QString LABEL_LAST_BACKUP_NAME = "Last Backup Name: ";
inline const QString LABEL_LAST_BACKUP_TIMESTAMP = "Last Backup Timestamp: ";
inline const QString LABEL_LAST_BACKUP_DURATION = "Last Backup Duration: ";
inline const QString LABEL_LAST_BACKUP_SIZE = "Last Backup Size: ";

// Tree View UI Settings
inline const QString STAGING_COLUMN_NAME = "Name";
constexpr int TREE_VIEW_START_HIDDEN_COLUMN = 1;
constexpr int TREE_VIEW_DEFAULT_COLUMN_COUNT = 4;

} // namespace UIConfig

#endif // UI_CONFIG_H
