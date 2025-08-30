// Project includes
#include "JsonServiceManager.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QSaveFile>

// Load JSON into QJsonObject
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

// Save QJsonObject to file
bool JsonManager::saveJsonFile(const QString& path, const QJsonObject& data) {
    return saveJsonFile(path, QJsonDocument(data));
}

// Load JSON into QJsonDocument
QJsonDocument JsonManager::loadJsonFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }

    return QJsonDocument::fromJson(file.readAll());
}

// Save QJsonDocument to file
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
