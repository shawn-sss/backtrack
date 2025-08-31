// Project includes
#include "TemplateServiceManager.h"
#include "TemplateServiceConstants.h"
#include "../PathServiceManager/PathServiceManager.h"
#include "../../../../constants/app_info.h"

// Qt includes
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Lifecycle
TemplateServiceManager::TemplateServiceManager(const QString& filePath, QObject* parent)
    : QObject(parent), m_filePath(filePath) {}

// Persistence
QJsonObject TemplateServiceManager::readJsonFile() const {
    QFile file(m_filePath);
    if (!file.exists()) return {};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return {};

    const QByteArray data = file.readAll();
    file.close();

    const QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.isObject() ? doc.object() : QJsonObject{};
}

bool TemplateServiceManager::writeJsonFile(const QJsonObject& obj) const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;

    const QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

// Mutation
bool TemplateServiceManager::saveTemplate(const QString& name, const QVector<TemplateEntry>& entries) {
    QJsonObject root = readJsonFile();

    QJsonArray listArray;
    QDateTime now = QDateTime::currentDateTimeUtc();
    for (TemplateEntry entry : entries) {
        entry.lastModified = now;
        listArray.append(entry.toJson());
    }

    root.insert(name, listArray);
    return writeJsonFile(root);
}

bool TemplateServiceManager::deleteTemplate(const QString& name) {
    QJsonObject root = readJsonFile();
    if (!root.contains(name)) return false;

    root.remove(name);

    if (root.value(TemplateServiceConstants::Keys::DefaultKey).toString() == name) {
        root.remove(TemplateServiceConstants::Keys::DefaultKey);
    }

    return writeJsonFile(root);
}

void TemplateServiceManager::setCurrentStagingEntries(const QVector<QString>& paths) {
    m_currentStagingEntries.clear();
    m_currentStagingEntries.reserve(paths.size());
    for (const QString& path : paths) {
        const QFileInfo info(path);
        m_currentStagingEntries.append({ path, info.isDir() });
    }
}

void TemplateServiceManager::restoreStaging(const QVector<TemplateEntry>& entries) {
    m_currentStagingEntries = entries;
}

// Query
QVector<TemplateEntry> TemplateServiceManager::loadTemplate(const QString& name) {
    QVector<TemplateEntry> result;
    const QJsonObject root = readJsonFile();
    if (!root.contains(name)) return result;

    const QJsonValue val = root.value(name);
    if (!val.isArray()) return result;

    const QJsonArray listArray = val.toArray();
    result.reserve(listArray.size());
    for (const QJsonValue& v : listArray) {
        if (v.isObject()) {
            result.append(TemplateEntry::fromJson(v.toObject()));
        }
    }
    return result;
}

QStringList TemplateServiceManager::listTemplates() const {
    const QJsonObject root = readJsonFile();
    QStringList keys = root.keys();
    keys.removeAll(TemplateServiceConstants::Keys::DefaultKey);
    return keys;
}

QVector<TemplateEntry> TemplateServiceManager::getCurrentStagingEntries() const {
    return m_currentStagingEntries;
}

// Defaults
void TemplateServiceManager::initializeDefaults() {
    const QString templateFilePath =
        PathServiceManager::appConfigFolderPath() + "/" +
        App::Items::k_APPDATA_SETUP_USER_TEMPLATES_FILE;

    QFile file(templateFilePath);
    if (file.exists())
        return;

    TemplateServiceManager manager(templateFilePath);
    manager.writeJsonFile(QJsonObject{});
}

QString TemplateServiceManager::getDefaultTemplate() const {
    const QJsonObject root = readJsonFile();
    return root.value(TemplateServiceConstants::Keys::DefaultKey).toString();
}

void TemplateServiceManager::setDefaultTemplate(const QString& name) {
    QJsonObject root = readJsonFile();
    root.insert(TemplateServiceConstants::Keys::DefaultKey, name);
    writeJsonFile(root);
}

void TemplateServiceManager::clearDefaultTemplate() {
    QJsonObject root = readJsonFile();
    root.remove(TemplateServiceConstants::Keys::DefaultKey);
    writeJsonFile(root);
}
