#ifndef USERSERVICEMANAGER_H
#define USERSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Handles loading, saving, and modifying user-specific settings
class UserServiceManager {
public:
    // Constructor
    explicit UserServiceManager(const QString& serviceFilePath);

    // File operations
    void load();
    void save() const;

    // Initializes default settings
    void initializeDefaults();

    // Access to user settings
    QJsonObject& settings();
    const QJsonObject& settings() const;

private:
    QString userServicePath;
    QJsonObject userSettings;
};

#endif // USERSERVICEMANAGER_H
