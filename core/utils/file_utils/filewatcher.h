#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QFileSystemWatcher>

class FileWatcher : public QObject {
    Q_OBJECT

public:
    explicit FileWatcher(QObject *parent = nullptr);
    ~FileWatcher() override = default;

    // Path management
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();

    // Retrieve watched paths
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Monitor changes
    void startWatching(const QString &rootPath);
    void updateWatchList(const QStringList &paths);

signals:
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    QFileSystemWatcher *watcher;
    QStringList watchList;
};

#endif // FILEWATCHER_H
