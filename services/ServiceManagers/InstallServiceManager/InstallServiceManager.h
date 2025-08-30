#ifndef INSTALLSERVICEMANAGER_H
#define INSTALLSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Handles persistent installation metadata
class InstallServiceManager {
public:
    explicit InstallServiceManager(const QString& metadataPath);

    // Persistence
    void load();
    void save();

    // Defaults
    static void initializeDefaults();

    // Access
    const QJsonObject& getMetadata() const;
    void setMetadata(const QJsonObject& metadata);

private:
    QString metadataFilePath;
    QJsonObject installMetadata;
};

#endif
