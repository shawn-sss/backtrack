#ifndef INSTALLCONFIGMANAGER_H
#define INSTALLCONFIGMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

class InstallConfigManager {
public:
    // Constructor
    explicit InstallConfigManager(const QString& metadataPath);

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

#endif // INSTALLCONFIGMANAGER_H
