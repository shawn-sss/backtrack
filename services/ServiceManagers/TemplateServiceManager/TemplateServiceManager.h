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

// Represents a single template entry
struct TemplateEntry {
    QString path;
    bool isFolder;

    QJsonObject toJson() const {
        return QJsonObject{
            { TemplateServiceConstants::Keys::Path, path },
            { TemplateServiceConstants::Keys::IsFolder, isFolder }
        };
    }

    static TemplateEntry fromJson(const QJsonObject& obj) {
        return {
            obj.value(TemplateServiceConstants::Keys::Path).toString(),
            obj.value(TemplateServiceConstants::Keys::IsFolder).toBool()
        };
    }
};

// Manages templates persisted to JSON
class TemplateServiceManager : public QObject {
    Q_OBJECT

public:
    explicit TemplateServiceManager(const QString& filePath, QObject* parent = nullptr);

    // Mutation
    bool saveTemplate(const QString& name, const QVector<TemplateEntry>& entries);
    bool deleteTemplate(const QString& name);
    void setCurrentStagingEntries(const QVector<QString>& paths);

    // Query
    QVector<TemplateEntry> loadTemplate(const QString& name);
    QStringList listTemplates() const;
    QVector<TemplateEntry> getCurrentStagingEntries() const;

private:
    QString m_filePath;
    QVector<TemplateEntry> m_currentStagingEntries;

    QJsonObject readJsonFile() const;
    bool writeJsonFile(const QJsonObject& obj) const;
};

#endif
