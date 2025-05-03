// Project includes
#include "filewatcher.h"
#include "../../../config/configsettings/_settings.h"

// Qt includes
#include <QFileInfo>
#include <QDir>
#include <QSet>
#include <QFileSystemWatcher>

// Constructor
FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Replaces the current watch list with a new set of paths
void FileWatcher::updateWatchList(const QStringList &paths) {
    removeAllPaths();
    addPaths(paths);
}

// Adds a single file or directory to the watch list
void FileWatcher::addPath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (!normalizedPath.isEmpty() &&
        !watcher->directories().contains(normalizedPath) &&
        !watcher->files().contains(normalizedPath) &&
        QFileInfo::exists(normalizedPath)) {
        watcher->addPath(normalizedPath);
    }
}

// Adds multiple files or directories to the watch list
void FileWatcher::addPaths(const QStringList &paths) {
    QStringList newPaths;

    QStringList directories = watcher->directories();
    QStringList files = watcher->files();

    QSet<QString> currentWatched = QSet<QString>(directories.begin(), directories.end())
                                       .unite(QSet<QString>(files.begin(), files.end()));

    for (const QString &path : paths) {
        QString normalizedPath = QDir::fromNativeSeparators(path);
        if (!normalizedPath.isEmpty() &&
            QFileInfo::exists(normalizedPath) &&
            !currentWatched.contains(normalizedPath)) {
            newPaths.append(normalizedPath);
        }
    }

    if (!newPaths.isEmpty()) {
        watcher->addPaths(newPaths);
    }
}

// Removes a single file or directory from the watch list
void FileWatcher::removePath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (watcher->directories().contains(normalizedPath) ||
        watcher->files().contains(normalizedPath)) {
        watcher->removePath(normalizedPath);
    }
}

// Removes all files and directories from the watch list
void FileWatcher::removeAllPaths() {
    QStringList allPaths = watcher->directories() + watcher->files();
    if (!allPaths.isEmpty()) {
        watcher->removePaths(allPaths);
    }
}

// Returns the list of currently watched directories
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Returns the list of currently watched files
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Starts watching a backup root and related subpaths
void FileWatcher::startWatching(const QString &rootPath) {
    removeAllPaths();

    QDir rootDir(rootPath);
    if (!rootDir.exists()) return;

    QSet<QString> pathsToWatch = {
        rootPath,
        rootDir.filePath(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER),
        rootDir.filePath(Backup::Infrastructure::k_BACKUP_SETUP_INFO_FILE),
        rootDir.filePath(Backup::Infrastructure::k_BACKUP_LOGS_FOLDER)
    };

    const QFileInfoList subDirs = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &dirInfo : subDirs) {
        pathsToWatch.insert(dirInfo.absoluteFilePath());
    }

    QStringList validPaths;
    for (const QString &path : pathsToWatch) {
        if (QFileInfo::exists(path)) {
            validPaths << path;
        }
    }

    addPaths(validPaths);
}
