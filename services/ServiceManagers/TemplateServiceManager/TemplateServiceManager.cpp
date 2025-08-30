// filename: templateservicemanager.cpp

#include "TemplateServiceManager.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>

TemplateServiceManager::TemplateServiceManager(const QString& filePath, QObject* parent)
    : QObject(parent), m_filePath(filePath) {}

QJsonObject TemplateServiceManager::readJsonFile() const {
    QFile file(m_filePath);
    if (!file.exists()) return QJsonObject();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QJsonObject();

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc.object();
}

bool TemplateServiceManager::writeJsonFile(const QJsonObject& obj) const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;

    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool TemplateServiceManager::saveTemplate(const QString& name, const QVector<TemplateEntry>& entries) {
    QJsonObject root = readJsonFile();
    QJsonArray listArray;
    for (const TemplateEntry& entry : entries) {
        listArray.append(entry.toJson());
    }
    root[name] = listArray;
    return writeJsonFile(root);
}

QVector<TemplateEntry> TemplateServiceManager::loadTemplate(const QString& name) {
    QVector<TemplateEntry> result;
    QJsonObject root = readJsonFile();
    if (!root.contains(name)) return result;

    QJsonArray listArray = root.value(name).toArray();
    for (const QJsonValue& val : listArray) {
        if (val.isObject()) {
            result.append(TemplateEntry::fromJson(val.toObject()));
        }
    }
    return result;
}

QStringList TemplateServiceManager::listTemplates() const {
    QJsonObject root = readJsonFile();
    return root.keys();
}

bool TemplateServiceManager::deleteTemplate(const QString& name) {
    QJsonObject root = readJsonFile();
    if (!root.contains(name)) return false;

    root.remove(name);
    return writeJsonFile(root);
}

void TemplateServiceManager::setCurrentStagingEntries(const QVector<QString>& paths) {
    m_currentStagingEntries.clear();
    for (const QString& path : paths) {
        QFileInfo info(path);
        m_currentStagingEntries.append({ path, info.isDir() });
    }
}

QVector<TemplateEntry> TemplateServiceManager::getCurrentStagingEntries() const {
    return m_currentStagingEntries;
}
