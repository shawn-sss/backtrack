#ifndef FILEWATCHER_H
#define FILEWATCHER_H

// Qt includes
#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>

// Monitors file and directory changes
class FileWatcher : public QObject {
    Q_OBJECT

public:
    // Constructs the file watcher
    explicit FileWatcher(QObject *parent = nullptr);

    // Replaces the current watch list with a new set of paths
    void updateWatchList(const QStringList &paths);

    // Adds a single path to the watch list
    void addPath(const QString &path);

    // Adds multiple paths to the watch list
    void addPaths(const QStringList &paths);

    // Removes a single path from the watch list
    void removePath(const QString &path);

    // Removes all currently watched paths
    void removeAllPaths();

    // Returns the list of currently watched directories
    QStringList watchedDirectories() const;

    // Returns the list of currently watched files
    QStringList watchedFiles() const;

    // Begins watching a root path and related folders/files
    void startWatching(const QString &rootPath);

signals:
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    QFileSystemWatcher *watcher;
    QStringList watchList;
};

#endif // FILEWATCHER_H
