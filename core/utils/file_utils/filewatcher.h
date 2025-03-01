#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QFileSystemWatcher>

// FileWatcher class definition
class FileWatcher : public QObject {
    Q_OBJECT

public:
    explicit FileWatcher(QObject *parent = nullptr);
    ~FileWatcher() override = default;

    // Path management
public:
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();

    // Retrieve watched paths
public:
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Monitoring and watch list updates
public:
    void startWatching(const QString &rootPath);
    void updateWatchList(const QStringList &paths);

    // Signals for file and directory changes
signals:
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

    // Internal variables
private:
    QFileSystemWatcher *watcher;
    QStringList watchList;
};

#endif // FILEWATCHER_H
