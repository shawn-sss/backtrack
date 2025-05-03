#ifndef FILEWATCHER_H
#define FILEWATCHER_H

// Qt includes
#include <QObject>
#include <QStringList>
#include <QFileSystemWatcher>

// Monitors file and directory changes
class FileWatcher : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit FileWatcher(QObject *parent = nullptr);

    // Updates the entire list of watched paths
    void updateWatchList(const QStringList &paths);

    // Watchlist management
    // Adds a single path
    void addPath(const QString &path);
    // Adds multiple paths
    void addPaths(const QStringList &paths);
    // Removes a single path
    void removePath(const QString &path);
    // Removes all paths
    void removeAllPaths();

    // Accessors for current watch state
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Begins watching a structured backup directory
    void startWatching(const QString &rootPath);

signals:
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    QFileSystemWatcher *watcher;
    QStringList watchList;
};

#endif // FILEWATCHER_H
