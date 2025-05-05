// Project includes
#include "jsonmanager.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QSaveFile>

// Loads a JSON file into a QJsonObject
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

// Saves a QJsonObject to a file
bool JsonManager::saveJsonFile(const QString& path, const QJsonObject& data) {
    return saveJsonFile(path, QJsonDocument(data));
}

// Loads a JSON file into a QJsonDocument (object or array)
QJsonDocument JsonManager::loadJsonFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QJsonDocument();
    }

    return QJsonDocument::fromJson(file.readAll());
}

// Saves a QJsonDocument to a file, ensuring the directory exists
bool JsonManager::saveJsonFile(const QString& path, const QJsonDocument& doc) {
    QDir().mkpath(QFileInfo(path).absolutePath());

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    if (file.write(doc.toJson(QJsonDocument::Indented)) == -1) {
        return false;
    }

    return file.commit();
}
