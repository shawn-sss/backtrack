#ifndef JSONMANAGER_H
#define JSONMANAGER_H

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

// Handles loading and saving JSON files
class JsonManager {
public:
    // Loads JSON into QJsonObject
    static bool loadJsonFile(const QString& path, QJsonObject& target);

    // Saves QJsonObject to file
    static bool saveJsonFile(const QString& path, const QJsonObject& data);

    // Loads raw QJsonDocument from file
    static QJsonDocument loadJsonFile(const QString& path);

    // Saves QJsonDocument to file
    static bool saveJsonFile(const QString& path, const QJsonDocument& doc);
};

#endif // JSONMANAGER_H
