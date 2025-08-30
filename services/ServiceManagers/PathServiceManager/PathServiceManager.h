#ifndef PATHSERVICEMANAGER_H
#define PATHSERVICEMANAGER_H

// Qt includes
#include <QString>

class PathServiceManager {
public:
    // Control
    static void setBackupDirectory(const QString& path);
    static QString getBackupDirectory();

    // Application data
    static QString appDataRootDir();
    static QString appConfigFolderPath();
    static QString appInitMetadataFilePath();
    static QString appNotificationsFilePath();
    static QString userSettingsFilePath();

    // Backup data
    static QString backupDataRootDir();
    static QString backupSetupFolderPath();
    static QString backupConfigFolderPath();
    static QString backupInitMetadataFilePath();
    static QString backupLogsFolderPath();
    static QString backupLogFilePath(const QString& backupName);
    static QString expectedBackupFolderPath(const QString& backupName);

private:
    static inline QString userBackupDir;
};

#endif
