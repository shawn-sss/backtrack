// Project includes
#include "JsonServiceManager.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QSaveFile>

// Load JSON content from a file into a QJsonObject
bool JsonManager::loadJsonFile(const QString& path, QJsonObject& target) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return false;
    }

    target = doc.object();
    return true;
}


// Save a QJsonObject to a file as JSON
bool JsonManager::saveJsonFile(const QString& path, const QJsonObject& data) {
    return saveJsonFile(path, QJsonDocument(data));
}


// Load JSON content from a file and return the QJsonDocument
QJsonDocument JsonManager::loadJsonFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }

    return QJsonDocument::fromJson(file.readAll());
}


// Save a QJsonDocument to a file with indentation
bool JsonManager::saveJsonFile(const QString& path, const QJsonDocument& doc) {
    QDir().mkpath(QFileInfo(path).absolutePath());

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    const QByteArray json = doc.toJson(QJsonDocument::Indented);
    if (file.write(json) == -1) {
        return false;
    }

    return file.commit();
}
