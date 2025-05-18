#ifndef PATHSERVICEMANAGER_H
#define PATHSERVICEMANAGER_H

#include <QString>

class PathServiceManager
{
public:
    // App data structure
    static QString appDataRootDir();
    static QString appConfigFolderPath();
    static QString appInitMetadataFilePath();
    static QString appNotificationsFilePath();
    static QString userSettingsFilePath();

    // Backup structure
    static QString backupDataRootDir();
    static QString backupSetupFolderPath();
    static QString backupConfigFolderPath();
    static QString backupInitMetadataFilePath();
    static QString backupLogsFolderPath();
    static QString backupLogFilePath(const QString& backupName);
    static QString expectedBackupFolderPath(const QString& backupName);
};

#endif // PATHSERVICEMANAGER_H
