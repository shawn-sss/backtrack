#include "filewatcher.h"
#include "../../config/_constants.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>

// Constructor & Initialization
FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Updates the watch list dynamically with normalized paths
void FileWatcher::updateWatchList(const QStringList &paths) {
    QStringList normalizedPaths;
    for (const QString &path : paths) {
        normalizedPaths.append(QDir::fromNativeSeparators(path));
    }
    watchList = normalizedPaths;
}

// Adds a path to the watcher after normalizing and checking for duplicates
void FileWatcher::addPath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (!normalizedPath.isEmpty() && !watchedDirectories().contains(normalizedPath)) {
        watcher->addPath(normalizedPath);
        //qDebug() << "Watching path: " << normalizedPath;
    }
}

// Adds multiple paths, ensuring no duplicates
void FileWatcher::addPaths(const QStringList &paths) {
    for (const QString &path : paths) {
        addPath(path);
    }
}

// Removes a path from the watcher
void FileWatcher::removePath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (watchedDirectories().contains(normalizedPath)) {
        watcher->removePath(normalizedPath);
        //qDebug() << "Stopped watching path: " << normalizedPath;
    }
}

// Removes all watched paths
void FileWatcher::removeAllPaths() {
    QStringList directories = watchedDirectories();
    if (!directories.isEmpty()) {
        watcher->removePaths(directories);
    }

    QStringList files = watchedFiles();
    if (!files.isEmpty()) {
        watcher->removePaths(files);
    }
}

// Retrieves currently watched directories
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Retrieves currently watched files
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Starts watching a directory and its subdirectories
void FileWatcher::startWatching(const QString &rootPath) {
    removeAllPaths();

    QString normalizedRootPath = QDir::fromNativeSeparators(rootPath);
    addPath(normalizedRootPath);  // Always watch the root backup directory

    QDir rootDir(normalizedRootPath);
    if (!rootDir.exists()) {
        qWarning() << "Directory does not exist, cannot watch:" << normalizedRootPath;
        return;
    }

    const QFileInfoList subDirectories = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Define important paths to watch dynamically
    watchList = {
        rootDir.filePath(AppConfig::BACKUP_CONFIG_FOLDER),
        rootDir.filePath(AppConfig::BACKUP_LOGS_DIRECTORY),
        rootDir.filePath(AppConfig::CONFIG_FILE_NAME)
    };

    // Add all subdirectories to the watch list
    for (const QFileInfo &dirInfo : subDirectories) {
        watchList.append(dirInfo.absoluteFilePath());
    }

    // Normalize paths and ensure they exist before adding them to the watcher
    QStringList validPaths;
    for (const QString &path : std::as_const(watchList)) {
        QString normalizedPath = QDir::fromNativeSeparators(path);
        if (QFileInfo::exists(normalizedPath) && !watchedDirectories().contains(normalizedPath)) {
            validPaths.append(normalizedPath);
        }
    }

    // Start watching valid paths
    addPaths(validPaths);

    // Debug output for watched paths
    //qDebug() << "Currently watching paths: " << watchedDirectories() + watchedFiles();
}
