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
    // Constructor and destructor
    explicit FileWatcher(QObject *parent = nullptr);
    ~FileWatcher() override = default;

    // Managing watched paths
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Start monitoring a directory
    void startWatching(const QString &rootPath);

signals:
    // Emitted when a directory or file changes
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    // File system watcher instance
    QFileSystemWatcher *watcher;
};

#endif // FILEWATCHER_H
