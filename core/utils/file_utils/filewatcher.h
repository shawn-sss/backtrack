#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileSystemWatcher>

// Class for monitoring file and directory changes
class FileWatcher : public QObject {
    Q_OBJECT

public:
    // Constructor and Destructor
    explicit FileWatcher(QObject *parent = nullptr);
    ~FileWatcher() override = default;

    // Managing Watched Paths
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Start Monitoring
    void startWatching(const QString &rootPath);

signals:
    // Signals for File/Directory Changes
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    // File System Watcher
    QFileSystemWatcher *watcher;
};

#endif // FILEWATCHER_H
