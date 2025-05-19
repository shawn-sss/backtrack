#ifndef BACKUP_CONFIG_H
#define BACKUP_CONFIG_H

// Qt includes
#include <QDir>
#include <QString>

// Drive label constants for backup item selection
namespace Backup::Drive {
constexpr auto k_DEFAULT_DRIVE_LABEL = "Local Disk";
constexpr auto k_DRIVE_LABEL_SUFFIX = "Drive";
}

// Timestamp formats for backup folders and display
namespace Backup::Timestamps {
constexpr auto k_BACKUP_FOLDER_TIMESTAMP_FORMAT = "_yyyyMMdd_HHmmss";
constexpr auto k_BACKUP_TIMESTAMP_DISPLAY_FORMAT = "MM/dd/yyyy hh:mm AP";
constexpr auto k_NOTIFICATION_TIMESTAMP_DISPLAY_FORMAT = "yyyy-MM-dd HH:mm:ss";
}

#endif // BACKUP_CONFIG_H
