#ifndef USERCONFIGMANAGER_H
#define USERCONFIGMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Handles loading, saving, and modifying user-specific settings
class UserConfigManager {
public:
    // Constructor
    explicit UserConfigManager(const QString& configFilePath);

    // File operations
    void load();
    void save() const;

    // Initializes default settings
    void initializeDefaults();

    // Access to user settings
    QJsonObject& settings();
    const QJsonObject& settings() const;

private:
    QString userConfigPath;
    QJsonObject userSettings;
};

#endif // USERCONFIGMANAGER_H
