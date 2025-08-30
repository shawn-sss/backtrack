// Project includes
#include "TemplateServiceManager.h"
#include "TemplateServiceConstants.h"

// Qt includes
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Lifecycle
TemplateServiceManager::TemplateServiceManager(const QString& filePath, QObject* parent)
    : QObject(parent), m_filePath(filePath) {}

// Persistence: read JSON
QJsonObject TemplateServiceManager::readJsonFile() const {
    QFile file(m_filePath);
    if (!file.exists()) return {};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return {};

    const QByteArray data = file.readAll();
    file.close();

    const QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.isObject() ? doc.object() : QJsonObject{};
}

// Persistence: write JSON
bool TemplateServiceManager::writeJsonFile(const QJsonObject& obj) const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;

    const QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

// Mutation: save template
bool TemplateServiceManager::saveTemplate(const QString& name, const QVector<TemplateEntry>& entries) {
    QJsonObject root = readJsonFile();

    QJsonArray listArray;
    for (const TemplateEntry& entry : entries) {
        listArray.append(entry.toJson());
    }

    root.insert(name, listArray);
    return writeJsonFile(root);
}

// Query: load template
QVector<TemplateEntry> TemplateServiceManager::loadTemplate(const QString& name) {
    QVector<TemplateEntry> result;
    const QJsonObject root = readJsonFile();
    if (!root.contains(name)) return result;

    const QJsonArray listArray = root.value(name).toArray();
    result.reserve(listArray.size());
    for (const QJsonValue& val : listArray) {
        if (val.isObject()) {
            result.append(TemplateEntry::fromJson(val.toObject()));
        }
    }
    return result;
}

// Query: list templates
QStringList TemplateServiceManager::listTemplates() const {
    const QJsonObject root = readJsonFile();
    return root.keys();
}

// Mutation: delete template
bool TemplateServiceManager::deleteTemplate(const QString& name) {
    QJsonObject root = readJsonFile();
    if (!root.contains(name)) return false;

    root.remove(name);
    return writeJsonFile(root);
}

// Mutation: set staging entries
void TemplateServiceManager::setCurrentStagingEntries(const QVector<QString>& paths) {
    m_currentStagingEntries.clear();
    m_currentStagingEntries.reserve(paths.size());
    for (const QString& path : paths) {
        const QFileInfo info(path);
        m_currentStagingEntries.append({ path, info.isDir() });
    }
}

// Query: get staging entries
QVector<TemplateEntry> TemplateServiceManager::getCurrentStagingEntries() const {
    return m_currentStagingEntries;
}
