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

    // Path Management Methods
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Directory Monitoring
    void startWatching(const QString &rootPath);

signals:
    // Signals for directory and file changes
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    // Member Variables
    QFileSystemWatcher *watcher;
};

#endif // FILEWATCHER_H
