#ifndef JSONMANAGER_H
#define JSONMANAGER_H

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

// Provides static methods for reading and writing JSON files
class JsonManager {
public:
    // Load JSON into QJsonObject
    static bool loadJsonFile(const QString& path, QJsonObject& target);

    // Save QJsonObject to file
    static bool saveJsonFile(const QString& path, const QJsonObject& data);

    // Load JSON into QJsonDocument
    static QJsonDocument loadJsonFile(const QString& path);

    // Save QJsonDocument to file
    static bool saveJsonFile(const QString& path, const QJsonDocument& doc);
};

#endif
