#ifndef INSTALLMETADATAMANAGER_H
#define INSTALLMETADATAMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Manages installation metadata: reading, writing, and initialization
class InstallMetadataManager {
public:
    // Lifecycle
    explicit InstallMetadataManager(const QString& metadataPath);

    // Metadata file operations
    void load();
    void save();

    // Initializes default metadata structure and persists it
    static void initializeDefaults();

    // Metadata accessors
    const QJsonObject& getMetadata() const;
    void setMetadata(const QJsonObject& metadata);

private:
    QString metadataFilePath;
    QJsonObject installMetadata;
};

#endif // INSTALLMETADATAMANAGER_H
