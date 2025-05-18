#ifndef FILEWATCHER_H
#define FILEWATCHER_H

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

    // Add/remove individual paths
    void addPath(const QString& path);
    void removePath(const QString& path);

    // Add/remove path collections
    void addPaths(const QStringList& paths);
    void removeAllPaths();
    void updateWatchList(const QStringList& paths);

    // Watch special config/backup paths
    void startWatchingMultiple(const QStringList& rootPaths);

    // Query watched paths
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

signals:
    void fileChanged(const QString& path);
    void directoryChanged(const QString& path);

private:
    QFileSystemWatcher* watcher;
};

#endif // FILEWATCHER_H
