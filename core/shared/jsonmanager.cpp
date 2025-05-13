// Project includes
#include "jsonmanager.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QSaveFile>

// Loads JSON into a QJsonObject from a file
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

// Saves a QJsonObject to file as JSON
bool JsonManager::saveJsonFile(const QString& path, const QJsonObject& data) {
    return saveJsonFile(path, QJsonDocument(data));
}

// Loads JSON document (object or array) from file
QJsonDocument JsonManager::loadJsonFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QJsonDocument();
    }

    return QJsonDocument::fromJson(file.readAll());
}

// Saves a QJsonDocument to file with formatting
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
