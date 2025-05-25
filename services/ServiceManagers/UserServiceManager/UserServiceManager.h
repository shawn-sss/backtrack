#ifndef USERSERVICEMANAGER_H
#define USERSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Manages user-specific settings
class UserServiceManager {
public:
    explicit UserServiceManager(const QString& serviceFilePath);

    // Load/save operations
    void load();
    void save() const;

    // Initialize settings with default values
    void initializeDefaults();

    // Accessors
    QJsonObject& settings();
    const QJsonObject& settings() const;

private:
    QString userServicePath;
    QJsonObject userSettings;
};

#endif // USERSERVICEMANAGER_H
