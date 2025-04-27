#ifndef INSTALLMETADATAMANAGER_H
#define INSTALLMETADATAMANAGER_H

// Qt includes
#include <QString>
#include <QJsonObject>

// Manages install metadata information
class InstallMetadataManager {
public:
    explicit InstallMetadataManager(const QString& metadataPath); // Constructor

    // Metadata file operations
    void load();
    void save();
    void setupDefaults(const QString& installDir);

    // Metadata accessors
    const QJsonObject& getMetadata() const;
    void setMetadata(const QJsonObject& metadata);

private:
    QString metadataFilePath; // File path to metadata file
    QJsonObject installMetadata; // In-memory install metadata
};

#endif // INSTALLMETADATAMANAGER_H
