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
    static QString backupConfigFolderPath();
    static QString backupInitMetadataFilePath();
    static QString backupLogsFolderPath();
};

#endif // PATHSERVICEMANAGER_H
