// Project includes
#include "installmetadatamanager.h"
#include "../../configsettings/_settings.h"
#include "../../../core/utils/file_utils/jsonmanager.h"

// Qt includes
#include <QDateTime>
#include <QStandardPaths>

// Constructor that sets the metadata file path
InstallMetadataManager::InstallMetadataManager(const QString& metadataPath)
    : metadataFilePath(metadataPath) {}

// Loads install metadata from file if available
void InstallMetadataManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(metadataFilePath, rootObject) && !rootObject.isEmpty()) {
        installMetadata = rootObject;
    }
}

// Saves the current install metadata to file
void InstallMetadataManager::save() {
    JsonManager::saveJsonFile(metadataFilePath, installMetadata);
}

// Initializes default metadata and writes it to disk
void InstallMetadataManager::initializeDefaults() {
    const QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
    + "/" + AppConfig::k_APPDATA_SETUP_FOLDER + "/" + AppConfig::k_APPDATA_SETUP_INFO_FILE;

    QJsonObject installData;
    installData["location"] = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    installData["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    QJsonObject metadata;
    metadata[InstallMetadataKeys::k_APP_AUTHOR] = AppInfo::k_APP_AUTHOR_NAME;
    metadata[InstallMetadataKeys::k_APP_NAME] = AppInfo::k_APP_NAME;
    metadata[InstallMetadataKeys::k_APP_VERSION] = AppInfo::k_APP_VERSION;
    metadata["install"] = installData;

    JsonManager::saveJsonFile(path, metadata);
}

// Returns a const reference to the install metadata
const QJsonObject& InstallMetadataManager::getMetadata() const {
    return installMetadata;
}

// Sets the install metadata object
void InstallMetadataManager::setMetadata(const QJsonObject& metadata) {
    installMetadata = metadata;
}
