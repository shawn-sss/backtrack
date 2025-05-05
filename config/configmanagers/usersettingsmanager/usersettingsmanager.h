#ifndef USERSETTINGSMANAGER_H
#define USERSETTINGSMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Handles loading, saving, and modifying user-specific settings
class UserSettingsManager {
public:
    // Lifecycle
    explicit UserSettingsManager(const QString& configFilePath);

    // File operations
    void load();
    void save() const;

    // Sets and saves default user settings
    void initializeDefaults();

    // Accessors for the settings JSON object
    QJsonObject& settings();
    const QJsonObject& settings() const;

private:
    QString userConfigPath;
    QJsonObject userSettings;
};

#endif // USERSETTINGSMANAGER_H
