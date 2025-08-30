#ifndef FILEWATCHERSERVICEMANAGER_H
#define FILEWATCHERSERVICEMANAGER_H

// Qt includes
#include <QObject>
#include <QStringList>

// Forward declaration (Qt class)
class QFileSystemWatcher;

// Monitors file and directory changes
class FileWatcher : public QObject {
    Q_OBJECT

public:
    explicit FileWatcher(QObject* parent = nullptr);

    // Manage single paths
    void addPath(const QString& path);
    void removePath(const QString& path);

    // Manage multiple paths
    void addPaths(const QStringList& paths);
    void removeAllPaths();
    void updateWatchList(const QStringList& paths);

    // Watch application folders and metadata
    void startWatchingMultiple(const QStringList& rootPaths);

    // Query current watch state
    [[nodiscard]] QStringList watchedDirectories() const;
    [[nodiscard]] QStringList watchedFiles() const;

signals:
    void fileChanged(const QString& path);
    void directoryChanged(const QString& path);

private:
    QFileSystemWatcher* watcher;
};

#endif
