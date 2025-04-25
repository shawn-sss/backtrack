// Project includes
#include "jsonmanager.h"

// Qt includes
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QDir>
#include <QDebug>

// Loads a JSON object from a file into the target
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

// Saves a JSON object to a file (creates dirs if needed)
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
