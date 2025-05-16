#ifndef FILEWATCHER_H
#define FILEWATCHER_H

// Qt includes
#include <QObject>
#include <QStringList>

class QFileSystemWatcher;

class FileWatcher : public QObject {
    Q_OBJECT

public:
    explicit FileWatcher(QObject* parent = nullptr);

    // Public interface for managing watched paths
    void addPath(const QString& path);
    void addPaths(const QStringList& paths);
    void removePath(const QString& path);
    void removeAllPaths();
    void updateWatchList(const QStringList& paths);

    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    void startWatching(const QString& rootPath);
    void startWatchingMultiple(const QStringList& rootPaths);

signals:
    void fileChanged(const QString& path);
    void directoryChanged(const QString& path);

private:
    QFileSystemWatcher* watcher;
};

#endif // FILEWATCHER_H
