// Project includes
#include "InstallServiceManager.h"
#include "InstallServiceConstants.h"
#include "../../../../core/shared/jsonmanager.h"

// Qt includes
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>

// Constructor sets metadata file path
InstallServiceManager::InstallServiceManager(const QString& metadataPath)
    : metadataFilePath(metadataPath) {}

// Loads metadata from file
void InstallServiceManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(metadataFilePath, rootObject) && !rootObject.isEmpty()) {
        installMetadata = rootObject;
    }
}

// Saves metadata to file
void InstallServiceManager::save() {
    JsonManager::saveJsonFile(metadataFilePath, installMetadata);
}

// Initializes default install metadata
void InstallServiceManager::initializeDefaults() {
    const QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    const QString fullPath = appDataPath + "/" +
                             InstallMetadataSettings::kMetadataFolder + "/" +
                             InstallMetadataSettings::kMetadataFile;

    QJsonObject installData;
    installData[InstallMetadataKeys::kInstallLocation] = appDataPath;
    installData[InstallMetadataKeys::kInstallTimeUtc] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    QJsonObject metadata;
    metadata[InstallMetadataKeys::kAppAuthor] = InstallMetadataSettings::kAppAuthor;
    metadata[InstallMetadataKeys::kAppName] = InstallMetadataSettings::kAppName;
    metadata[InstallMetadataKeys::kAppVersion] = InstallMetadataSettings::kAppVersion;
    metadata[InstallMetadataKeys::kInstallSection] = installData;

    JsonManager::saveJsonFile(fullPath, metadata);
}

// Returns stored metadata object
const QJsonObject& InstallServiceManager::getMetadata() const {
    return installMetadata;
}

// Updates metadata with new object
void InstallServiceManager::setMetadata(const QJsonObject& metadata) {
    installMetadata = metadata;
}
