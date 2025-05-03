#ifndef USERSETTINGSMANAGER_H
#define USERSETTINGSMANAGER_H

// Qt includes
#include <QString>
#include <QJsonObject>

// Manages user settings storage and access
class UserSettingsManager {
public:
    // Constructor
    explicit UserSettingsManager(const QString& configFilePath);

    // File operations
    void load();
    void save() const;

    // Access to settings object
    QJsonObject& settings();
    const QJsonObject& settings() const;

private:
    // Path to the user config file
    QString userConfigPath;

    // In-memory user settings
    QJsonObject userSettings;
};

#endif // USERSETTINGSMANAGER_H
