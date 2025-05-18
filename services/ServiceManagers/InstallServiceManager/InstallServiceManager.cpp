#include "InstallServiceManager.h"
#include "InstallServiceConstants.h"
#include "../../../../core/shared/jsonmanager.h"

// Qt includes
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>

// Loads and manages install service metadata
InstallServiceManager::InstallServiceManager(const QString& metadataPath)
    : metadataFilePath(metadataPath) {}

// Loads metadata from file
void InstallServiceManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(metadataFilePath, rootObject) && !rootObject.isEmpty()) {
        installMetadata = rootObject;
    }
}

// Saves current metadata to file
void InstallServiceManager::save() {
    JsonManager::saveJsonFile(metadataFilePath, installMetadata);
}

// Initializes metadata with default install values
void InstallServiceManager::initializeDefaults() {
    const QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    const QString fullPath = appDataPath + "/" +
                             InstallMetadataSettings::kMetadataFolder + "/" +
                             InstallMetadataSettings::kMetadataFile;

    QJsonObject installData;
    installData[InstallMetadataKeys::kInstallLocation] = appDataPath;
    installData[InstallMetadataKeys::kInstallTimeUtc] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    QJsonObject metadata;
    metadata[InstallMetadataKeys::kAppName] = InstallMetadataSettings::kAppName;
    metadata[InstallMetadataKeys::kAppAuthor] = InstallMetadataSettings::kAppAuthor;
    metadata[InstallMetadataKeys::kAppVersion] = InstallMetadataSettings::kAppVersion;
    metadata[InstallMetadataKeys::kInstallSection] = installData;

    JsonManager::saveJsonFile(fullPath, metadata);
}

// Returns the current metadata
const QJsonObject& InstallServiceManager::getMetadata() const {
    return installMetadata;
}

// Sets the metadata to a new object
void InstallServiceManager::setMetadata(const QJsonObject& metadata) {
    installMetadata = metadata;
}
