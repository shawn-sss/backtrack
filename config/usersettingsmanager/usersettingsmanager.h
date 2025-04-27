#ifndef USERSETTINGSMANAGER_H
#define USERSETTINGSMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Manages user settings storage and access
class UserSettingsManager {
public:
    explicit UserSettingsManager(const QString& configFilePath); // Constructor

    // Settings file operations
    void load();
    void save() const;

    // Accessors for user settings
    QJsonObject& settings();
    const QJsonObject& settings() const;

private:
    QString userConfigPath; // Path to user config file
    QJsonObject userSettings; // In-memory user settings
};

#endif // USERSETTINGSMANAGER_H
