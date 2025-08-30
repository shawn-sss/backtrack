// filename: templateservicemanager.h

#ifndef TEMPLATESERVICEMANAGER_H
#define TEMPLATESERVICEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QJsonObject>

struct TemplateEntry {
    QString path;
    bool isFolder;

    QJsonObject toJson() const {
        return QJsonObject{
            { "path", path },
            { "isFolder", isFolder }
        };
    }

    static TemplateEntry fromJson(const QJsonObject& obj) {
        return TemplateEntry{
            obj.value("path").toString(),
            obj.value("isFolder").toBool()
        };
    }
};

class TemplateServiceManager : public QObject {
    Q_OBJECT

public:
    explicit TemplateServiceManager(const QString& filePath, QObject* parent = nullptr);

    bool saveTemplate(const QString& name, const QVector<TemplateEntry>& entries);
    QVector<TemplateEntry> loadTemplate(const QString& name);
    QStringList listTemplates() const;
    bool deleteTemplate(const QString& name);

    void setCurrentStagingEntries(const QVector<QString>& paths);
    QVector<TemplateEntry> getCurrentStagingEntries() const;

private:
    QString m_filePath;
    QVector<TemplateEntry> m_currentStagingEntries;

    QJsonObject readJsonFile() const;
    bool writeJsonFile(const QJsonObject& obj) const;
};

#endif // TEMPLATESERVICEMANAGER_H
