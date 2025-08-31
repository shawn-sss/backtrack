#ifndef TEMPLATESERVICEMANAGER_H
#define TEMPLATESERVICEMANAGER_H

// Project includes
#include "TemplateServiceConstants.h"

// Qt includes
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QJsonObject>
#include <QDateTime>

// Entry
struct TemplateEntry {
    QString path;
    bool isFolder;
    QDateTime lastModified;

    QJsonObject toJson() const {
        QJsonObject obj{
            { TemplateServiceConstants::Keys::Path, path },
            { TemplateServiceConstants::Keys::IsFolder, isFolder }
        };
        if (lastModified.isValid()) {
            obj.insert("lastModified", lastModified.toString(Qt::ISODate));
        }
        return obj;
    }

    static TemplateEntry fromJson(const QJsonObject& obj) {
        return {
            obj.value(TemplateServiceConstants::Keys::Path).toString(),
            obj.value(TemplateServiceConstants::Keys::IsFolder).toBool(),
            QDateTime::fromString(obj.value("lastModified").toString(), Qt::ISODate)
        };
    }
};

// Manager
class TemplateServiceManager : public QObject {
    Q_OBJECT

public:
    explicit TemplateServiceManager(const QString& filePath, QObject* parent = nullptr);

    // Mutation
    bool saveTemplate(const QString& name, const QVector<TemplateEntry>& entries);
    bool deleteTemplate(const QString& name);
    void setCurrentStagingEntries(const QVector<QString>& paths);
    void restoreStaging(const QVector<TemplateEntry>& entries);

    // Query
    QVector<TemplateEntry> loadTemplate(const QString& name);
    QStringList listTemplates() const;
    QVector<TemplateEntry> getCurrentStagingEntries() const;

    // Defaults
    static void initializeDefaults();
    QString getDefaultTemplate() const;
    void setDefaultTemplate(const QString& name);
    void clearDefaultTemplate();

private:
    QString m_filePath;
    QVector<TemplateEntry> m_currentStagingEntries;

    QJsonObject readJsonFile() const;
    bool writeJsonFile(const QJsonObject& obj) const;
};

#endif
