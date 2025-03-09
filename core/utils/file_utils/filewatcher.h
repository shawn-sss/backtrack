#ifndef FILEWATCHER_H
#define FILEWATCHER_H

// Built-in Qt includes
#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>

// Monitors file and directory changes
class FileWatcher : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit FileWatcher(QObject *parent = nullptr);

    // Watchlist management
    void updateWatchList(const QStringList &paths);
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();

    // Retrieve watched paths
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Starts monitoring a root path
    void startWatching(const QString &rootPath);

signals:
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    QFileSystemWatcher *watcher;
    QStringList watchList;
};

#endif // FILEWATCHER_H
