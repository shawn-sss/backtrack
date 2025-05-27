#include "snaplistservicemanager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>

SnapListServiceManager::SnapListServiceManager(const QString& filePath, QObject* parent)
    : QObject(parent), m_filePath(filePath) {}

QJsonObject SnapListServiceManager::readJsonFile() const {
    QFile file(m_filePath);
    if (!file.exists()) return QJsonObject();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QJsonObject();

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc.object();
}

bool SnapListServiceManager::writeJsonFile(const QJsonObject& obj) const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;

    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool SnapListServiceManager::saveSnapList(const QString& name, const QVector<SnapListEntry>& entries) {
    QJsonObject root = readJsonFile();
    QJsonArray listArray;
    for (const SnapListEntry& entry : entries) {
        listArray.append(entry.toJson());
    }
    root[name] = listArray;
    return writeJsonFile(root);
}

QVector<SnapListEntry> SnapListServiceManager::loadSnapList(const QString& name) {
    QVector<SnapListEntry> result;
    QJsonObject root = readJsonFile();
    if (!root.contains(name)) return result;

    QJsonArray listArray = root.value(name).toArray();
    for (const QJsonValue& val : listArray) {
        if (val.isObject()) {
            result.append(SnapListEntry::fromJson(val.toObject()));
        }
    }
    return result;
}

QStringList SnapListServiceManager::listSnapLists() const {
    QJsonObject root = readJsonFile();
    return root.keys();
}

bool SnapListServiceManager::deleteSnapList(const QString& name) {
    QJsonObject root = readJsonFile();
    if (!root.contains(name)) return false;

    root.remove(name);
    return writeJsonFile(root);
}

void SnapListServiceManager::setCurrentStagingEntries(const QVector<QString>& paths) {
    m_currentStagingEntries.clear();
    for (const QString& path : paths) {
        QFileInfo info(path);
        m_currentStagingEntries.append({ path, info.isDir() });
    }
}

QVector<SnapListEntry> SnapListServiceManager::getCurrentStagingEntries() const {
    return m_currentStagingEntries;
}
