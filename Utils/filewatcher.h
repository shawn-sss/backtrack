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

    // Methods for managing watched paths
    void addPath(const QString &path);
    void addPaths(const QStringList &paths);
    void removePath(const QString &path);
    void removeAllPaths();
    QStringList watchedDirectories() const;
    QStringList watchedFiles() const;

    // Method to start monitoring a directory and its contents
    void startWatching(const QString &rootPath);

signals:
    // Signals for changes in directories and files
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);

private:
    // Member variables
    QFileSystemWatcher *watcher;
};

#endif // FILEWATCHER_H
