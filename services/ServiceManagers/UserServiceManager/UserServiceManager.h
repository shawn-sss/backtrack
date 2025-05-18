#ifndef USERSERVICEMANAGER_H
#define USERSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Manages user-specific settings, including loading, saving, and defaults
class UserServiceManager {
public:
    explicit UserServiceManager(const QString& serviceFilePath);

    // Loads settings from file
    void load();

    // Saves current settings to file
    void save() const;

    // Initializes settings with default values
    void initializeDefaults();

    // Accessors
    QJsonObject& settings();
    const QJsonObject& settings() const;

private:
    QString userServicePath;
    QJsonObject userSettings;
};

#endif // USERSERVICEMANAGER_H
