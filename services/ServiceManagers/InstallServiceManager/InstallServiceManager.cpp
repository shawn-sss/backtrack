// Project includes
#include "InstallServiceManager.h"
#include "InstallServiceConstants.h"
#include "../JsonServiceManager/JsonServiceManager.h"

// Qt includes
#include <QDateTime>
#include <QStandardPaths>

// Lifecycle: construct manager
InstallServiceManager::InstallServiceManager(const QString& metadataPath)
    : metadataFilePath(metadataPath) {}

// Persistence: load metadata from file
void InstallServiceManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(metadataFilePath, rootObject) && !rootObject.isEmpty()) {
        installMetadata = rootObject;
    }
}

// Persistence: save metadata to file
void InstallServiceManager::save() {
    JsonManager::saveJsonFile(metadataFilePath, installMetadata);
}

// Defaults: initialize install metadata
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

// Access: get metadata
const QJsonObject& InstallServiceManager::getMetadata() const {
    return installMetadata;
}

// Access: set metadata
void InstallServiceManager::setMetadata(const QJsonObject& metadata) {
    installMetadata = metadata;
}
