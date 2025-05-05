#ifndef JSONMANAGER_H
#define JSONMANAGER_H

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

// Provides utility methods for loading and saving JSON files
class JsonManager {
public:
    // Load JSON data into a QJsonObject
    static bool loadJsonFile(const QString& path, QJsonObject& target);

    // Save a QJsonObject to a file
    static bool saveJsonFile(const QString& path, const QJsonObject& data);

    // Load JSON data into a QJsonDocument (object or array)
    static QJsonDocument loadJsonFile(const QString& path);

    // Save a QJsonDocument to a file
    static bool saveJsonFile(const QString& path, const QJsonDocument& doc);
};

#endif // JSONMANAGER_H
