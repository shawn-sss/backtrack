// Project includes same directory
#include "jsonmanager.h"

// Built-in Qt includes
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QDir>
#include <QDebug>

// Load JSON from file
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

// Save JSON to file
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
