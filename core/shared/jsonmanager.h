#ifndef JSONMANAGER_H
#define JSONMANAGER_H

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

// Provides static methods for reading and writing JSON files
class JsonManager {
public:
    // Load JSON into a QJsonObject
    static bool loadJsonFile(const QString& path, QJsonObject& target);

    // Save a QJsonObject to a JSON file
    static bool saveJsonFile(const QString& path, const QJsonObject& data);

    // Load and return a full QJsonDocument from file
    static QJsonDocument loadJsonFile(const QString& path);

    // Save a full QJsonDocument to file
    static bool saveJsonFile(const QString& path, const QJsonDocument& doc);
};

#endif // JSONMANAGER_H
