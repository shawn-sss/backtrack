#ifndef PATHSERVICEMANAGER_H
#define PATHSERVICEMANAGER_H

// Qt includes
#include <QString>

class PathServiceManager {
public:
    // Set and get the backup directory
    static void setBackupDirectory(const QString& path);
    static QString getBackupDirectory();

    // Application data and configuration paths
    static QString appDataRootDir();
    static QString appConfigFolderPath();
    static QString appInitMetadataFilePath();
    static QString appNotificationsFilePath();
    static QString userSettingsFilePath();

    // Backup directory structure
    static QString backupDataRootDir();
    static QString backupSetupFolderPath();
    static QString backupConfigFolderPath();
    static QString backupInitMetadataFilePath();
    static QString backupLogsFolderPath();
    static QString backupLogFilePath(const QString& backupName);
    static QString expectedBackupFolderPath(const QString& backupName);

private:
    // Holds the user-specified backup directory path
    static inline QString userBackupDir;
};

#endif // PATHSERVICEMANAGER_H
