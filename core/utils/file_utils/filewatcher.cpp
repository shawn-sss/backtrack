// Project includes same directory
#include "filewatcher.h"

// Project includes different directory
#include "../../../config/_constants.h"

// Built-in Qt includes
#include <QDir>
#include <QFileInfo>
#include <QSet>

// Constructor
FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Updates the watch list with new paths
void FileWatcher::updateWatchList(const QStringList &paths) {
    removeAllPaths();
    addPaths(paths);
}

// Adds a single path to the watcher
void FileWatcher::addPath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (!normalizedPath.isEmpty() &&
        !watcher->directories().contains(normalizedPath) &&
        !watcher->files().contains(normalizedPath) &&
        QFileInfo::exists(normalizedPath)) {
        watcher->addPath(normalizedPath);
    }
}

// Adds multiple paths to the watcher
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

// Removes a single path from the watcher
void FileWatcher::removePath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (watcher->directories().contains(normalizedPath) ||
        watcher->files().contains(normalizedPath)) {
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

// Starts watching directories and files under the given root path
void FileWatcher::startWatching(const QString &rootPath) {
    removeAllPaths();

    QDir rootDir(rootPath);
    if (!rootDir.exists()) return;

    QSet<QString> pathsToWatch = {
        rootPath,
        rootDir.filePath(AppConfig::BACKUP_SETUP_FOLDER),
        rootDir.filePath(AppConfig::BACKUP_SETUP_LOGS_FOLDER),
        rootDir.filePath(AppConfig::BACKUP_SETUP_INFO_FILE)
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
