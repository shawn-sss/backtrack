#ifndef USERSERVICEMANAGER_H
#define USERSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Manages user-specific settings
class UserServiceManager {
public:
    explicit UserServiceManager(const QString& serviceFilePath);

    // Persistence
    void load();
    void save() const;
    void initializeDefaults();

    // Accessors
    QJsonObject& settings();
    const QJsonObject& settings() const;

private:
    QString userServicePath;
    QJsonObject userSettings;
};

#endif
