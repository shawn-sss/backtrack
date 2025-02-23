#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileSystemWatcher>

// File and directory monitoring utility
class FileWatcher : public QObject {
    Q_OBJECT

public:
    // Constructor and Destructor
    explicit FileWatcher(QObject *parent = nullptr);
    ~FileWatcher() override = default;

    // Path Management
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();

    // Retrieval Methods
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Monitoring Functionality
    void startWatching(const QString &rootPath);
    void updateWatchList(const QStringList &paths); // New method to update the watch list dynamically

signals:
    // Change Detection Signals
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    // Internal Watcher
    QFileSystemWatcher *watcher;

    // List of paths being watched
    QStringList watchList;
};

#endif // FILEWATCHER_H
