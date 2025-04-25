#ifndef JSONMANAGER_H
#define JSONMANAGER_H

// Qt includes
#include <QString>
#include <QJsonObject>

class JsonManager {
public:
    // Loads a JSON object from the given file path
    static bool loadJsonFile(const QString& path, QJsonObject& target);

    // Saves a JSON object to the given file path
    static bool saveJsonFile(const QString& path, const QJsonObject& data);
};

#endif // JSONMANAGER_H
