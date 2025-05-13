#ifndef FILEWATCHER_H
#define FILEWATCHER_H

// Qt includes
#include <QObject>
#include <QStringList>
#include <QFileSystemWatcher>

// Monitors files and directories for changes
class FileWatcher : public QObject {
    Q_OBJECT

public:
    // Constructor
    explicit FileWatcher(QObject* parent = nullptr);

    // Replaces entire watch list
    void updateWatchList(const QStringList& paths);

    // Watch management
    void addPath(const QString& path);
    void addPaths(const QStringList& paths);
    void removePath(const QString& path);
    void removeAllPaths();

    // Access watched items
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Begins watching structured backup directory
    void startWatching(const QString& rootPath);

signals:
    void directoryChanged(const QString& path);
    void fileChanged(const QString& path);

private:
    QFileSystemWatcher* watcher;
    QStringList watchList;
};

#endif // FILEWATCHER_H
