#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <QString>

namespace UserSettings {
inline const QString DEFAULT_BACKUP_DESTINATION = "C:\\temp";
inline const QString BACKUP_FOLDER_PREFIX = "BKP_";
inline const QString BACKUP_SUMMARY_FILENAME = ".backup_summary.json";
}

#endif // USER_SETTINGS_H
