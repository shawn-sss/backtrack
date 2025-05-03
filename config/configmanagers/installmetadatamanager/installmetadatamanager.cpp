// Project includes
#include "../../configsettings/_settings.h"
#include "../../../core/utils/file_utils/jsonmanager.h"
#include "installmetadatamanager.h"

// Qt includes
#include <QDateTime>

// Constructor
InstallMetadataManager::InstallMetadataManager(const QString& metadataPath)
    : metadataFilePath(metadataPath) {}

// Load install metadata from file
void InstallMetadataManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(metadataFilePath, rootObject) && !rootObject.isEmpty()) {
        installMetadata = rootObject;
    }
}

// Save install metadata to file
void InstallMetadataManager::save() {
    JsonManager::saveJsonFile(metadataFilePath, installMetadata);
}

// Set up default install metadata
void InstallMetadataManager::setupDefaults(const QString& installDir) {
    QJsonObject installData;
    installData["location"] = installDir;
    installData["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    installMetadata[InstallMetadataKeys::k_APP_AUTHOR] = AppInfo::k_APP_AUTHOR_NAME;
    installMetadata[InstallMetadataKeys::k_APP_NAME] = AppInfo::k_APP_NAME;
    installMetadata[InstallMetadataKeys::k_APP_VERSION] = AppInfo::k_APP_VERSION;
    installMetadata["install"] = installData;

    save();
}

// Return the current install metadata
const QJsonObject& InstallMetadataManager::getMetadata() const {
    return installMetadata;
}

// Set the install metadata manually
void InstallMetadataManager::setMetadata(const QJsonObject& metadata) {
    installMetadata = metadata;
}
