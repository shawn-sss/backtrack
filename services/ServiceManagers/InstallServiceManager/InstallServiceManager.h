#ifndef INSTALLSERVICEMANAGER_H
#define INSTALLSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Manages application installation metadata
class InstallServiceManager {
public:
    // Constructor
    explicit InstallServiceManager(const QString& metadataPath);

    // File operations
    void load();
    void save();

    // Default initialization
    static void initializeDefaults();

    // Metadata access
    const QJsonObject& getMetadata() const;
    void setMetadata(const QJsonObject& metadata);

private:
    // Path to metadata file
    QString metadataFilePath;

    // Loaded metadata
    QJsonObject installMetadata;
};

#endif // INSTALLSERVICEMANAGER_H
