#ifndef INSTALLMETADATAMANAGER_H
#define INSTALLMETADATAMANAGER_H

// Qt includes
#include <QString>
#include <QJsonObject>

// Manages install metadata information
class InstallMetadataManager {
public:
    // Constructor
    explicit InstallMetadataManager(const QString& metadataPath);

    // File operations
    void load();
    void save();
    void setupDefaults(const QString& installDir);

    // Metadata access
    const QJsonObject& getMetadata() const;
    void setMetadata(const QJsonObject& metadata);

private:
    // Path to the metadata JSON file
    QString metadataFilePath;

    // Cached install metadata
    QJsonObject installMetadata;
};

#endif // INSTALLMETADATAMANAGER_H
