#ifndef SNAPLISTSERVICEMANAGER_H
#define SNAPLISTSERVICEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QJsonObject>
#include <QFileInfo>

struct SnapListEntry {
    QString path;
    bool isFolder;

    QJsonObject toJson() const {
        return QJsonObject{
            { "path", path },
            { "isFolder", isFolder }
        };
    }

    static SnapListEntry fromJson(const QJsonObject& obj) {
        return SnapListEntry{
            obj.value("path").toString(),
            obj.value("isFolder").toBool()
        };
    }
};

class SnapListServiceManager : public QObject {
    Q_OBJECT

public:
    explicit SnapListServiceManager(const QString& filePath, QObject* parent = nullptr);

    bool saveSnapList(const QString& name, const QVector<SnapListEntry>& entries);
    QVector<SnapListEntry> loadSnapList(const QString& name);
    QStringList listSnapLists() const;
    bool deleteSnapList(const QString& name);

    void setCurrentStagingEntries(const QVector<QString>& paths);
    QVector<SnapListEntry> getCurrentStagingEntries() const;

private:
    QString m_filePath;
    QVector<SnapListEntry> m_currentStagingEntries;

    QJsonObject readJsonFile() const;
    bool writeJsonFile(const QJsonObject& obj) const;
};

#endif // SNAPLISTSERVICEMANAGER_H
