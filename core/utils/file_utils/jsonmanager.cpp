// Project includes
#include "jsonmanager.h"

// Qt includes
#include <QFile>
#include <QDir>
#include <QSaveFile>
#include <QJsonDocument>

// Loads a JSON object from a file into the given target object
bool JsonManager::loadJsonFile(const QString& path, QJsonObject& target) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return false;
    }
    target = doc.object();
    return true;
}

// Saves a JSON object to a file, creating directories as needed
bool JsonManager::saveJsonFile(const QString& path, const QJsonObject& data) {
    QDir().mkpath(QFileInfo(path).absolutePath());
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    if (file.write(QJsonDocument(data).toJson(QJsonDocument::Indented)) == -1) {
        return false;
    }
    return file.commit();
}
