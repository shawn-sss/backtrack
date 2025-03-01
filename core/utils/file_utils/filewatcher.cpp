#include "filewatcher.h"

#include "../../../config/_constants.h"

#include <QDir>
#include <QSet>
#include <QFileInfo>

// Converts QStringList to QSet<QString>
static QSet<QString> toSet(const QStringList &list) {
    QSet<QString> result;
    for (const QString &item : list) {
        result.insert(item);
    }
    return result;
}

// Constructor
FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Updates the internal watch list
void FileWatcher::updateWatchList(const QStringList &paths) {
    watchList.clear();
    watchList.reserve(paths.size());
    for (const QString &path : paths) {
        watchList.append(QDir::fromNativeSeparators(path));
    }
}

// Adds a single path to the watcher
void FileWatcher::addPath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (!normalizedPath.isEmpty()) {
        const QStringList currentWatched = watchedDirectories();
        if (!currentWatched.contains(normalizedPath)) {
            watcher->addPath(normalizedPath);
        }
    }
}

// Adds multiple paths to the watcher
void FileWatcher::addPaths(const QStringList &paths) {
    QStringList newPaths;
    newPaths.reserve(paths.size());

    const QSet<QString> currentWatched = toSet(watchedDirectories());

    for (const QString &path : paths) {
        QString normalizedPath = QDir::fromNativeSeparators(path);
        if (!currentWatched.contains(normalizedPath)) {
            newPaths.append(normalizedPath);
        }
    }

    if (!newPaths.isEmpty()) {
        watcher->addPaths(newPaths);
    }
}

// Removes a single path from the watcher
void FileWatcher::removePath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    const QStringList currentWatched = watchedDirectories();

    if (currentWatched.contains(normalizedPath)) {
        watcher->removePath(normalizedPath);
    }
}

// Removes all watched paths
void FileWatcher::removeAllPaths() {
    QStringList allPaths = watcher->directories() + watcher->files();
    if (!allPaths.isEmpty()) {
        watcher->removePaths(allPaths);
    }
}

// Returns the list of watched directories
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Returns the list of watched files
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Starts watching the root directory and important subdirectories
void FileWatcher::startWatching(const QString &rootPath) {
    removeAllPaths();

    QString normalizedRootPath = QDir::fromNativeSeparators(rootPath);
    addPath(normalizedRootPath);

    QDir rootDir(normalizedRootPath);
    if (!rootDir.exists()) {
        return;
    }

    QSet<QString> pathsToWatch;
    pathsToWatch.insert(rootDir.filePath(AppConfig::BACKUP_CONFIG_FOLDER));
    pathsToWatch.insert(rootDir.filePath(AppConfig::BACKUP_LOGS_DIRECTORY));
    pathsToWatch.insert(rootDir.filePath(AppConfig::CONFIG_FILE_NAME));

    for (const QFileInfo &dirInfo : rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        pathsToWatch.insert(dirInfo.absoluteFilePath());
    }

    QStringList validPaths;
    const QSet<QString> currentlyWatched = toSet(watchedDirectories());

    for (const QString &path : pathsToWatch) {
        if (QFileInfo::exists(path) && !currentlyWatched.contains(path)) {
            validPaths.append(path);
        }
    }

    addPaths(validPaths);
}
