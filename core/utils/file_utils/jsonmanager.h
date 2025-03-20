#ifndef JSONMANAGER_H
#define JSONMANAGER_H

// Built-in Qt includes
#include <QString>
#include <QJsonObject>

class JsonManager {
public:
    // Load JSON from file
    static bool loadJsonFile(const QString& path, QJsonObject& target);

    // Save JSON to file
    static bool saveJsonFile(const QString& path, const QJsonObject& data);
};

#endif // JSONMANAGER_H
