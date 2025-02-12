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

    // Path management
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();

    // Retrieval methods
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Monitoring functionality
    void startWatching(const QString &rootPath);

signals:
    // Change detection signals
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    QFileSystemWatcher *watcher;
};

#endif // FILEWATCHER_H
