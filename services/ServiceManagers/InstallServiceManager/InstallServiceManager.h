#ifndef INSTALLSERVICEMANAGER_H
#define INSTALLSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Handles persistent installation metadata
class InstallServiceManager {
public:
    explicit InstallServiceManager(const QString& metadataPath);

    // Load and save metadata
    void load();
    void save();

    // Initialize metadata with default values
    static void initializeDefaults();

    // Metadata accessors
    const QJsonObject& getMetadata() const;
    void setMetadata(const QJsonObject& metadata);

private:
    QString metadataFilePath;
    QJsonObject installMetadata;
};

#endif // INSTALLSERVICEMANAGER_H
