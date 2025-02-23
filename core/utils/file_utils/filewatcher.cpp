#include "filewatcher.h"
#include "../../../config/_constants.h"

#include <QDir>
#include <QFileInfo>

FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Update watch list
void FileWatcher::updateWatchList(const QStringList &paths) {
    QStringList normalizedPaths;
    for (const QString &path : paths) {
        normalizedPaths.append(QDir::fromNativeSeparators(path));
    }
    watchList = normalizedPaths;
}

// Add path to watcher
void FileWatcher::addPath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (!normalizedPath.isEmpty() && !watchedDirectories().contains(normalizedPath)) {
        watcher->addPath(normalizedPath);
    }
}

// Add multiple paths
void FileWatcher::addPaths(const QStringList &paths) {
    for (const QString &path : paths) {
        addPath(path);
    }
}

// Remove path from watcher
void FileWatcher::removePath(const QString &path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (watchedDirectories().contains(normalizedPath)) {
        watcher->removePath(normalizedPath);
    }
}

// Remove all watched paths
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

// Retrieve watched directories
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Retrieve watched files
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Start monitoring directory
void FileWatcher::startWatching(const QString &rootPath) {
    removeAllPaths();
    QString normalizedRootPath = QDir::fromNativeSeparators(rootPath);
    addPath(normalizedRootPath);

    QDir rootDir(normalizedRootPath);
    if (!rootDir.exists()) {
        return;
    }

    const QFileInfoList subDirectories = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    watchList = {
        rootDir.filePath(AppConfig::BACKUP_CONFIG_FOLDER),
        rootDir.filePath(AppConfig::BACKUP_LOGS_DIRECTORY),
        rootDir.filePath(AppConfig::CONFIG_FILE_NAME)
    };

    for (const QFileInfo &dirInfo : subDirectories) {
        watchList.append(dirInfo.absoluteFilePath());
    }

    QStringList validPaths;
    for (const QString &path : std::as_const(watchList)) {
        QString normalizedPath = QDir::fromNativeSeparators(path);
        if (QFileInfo::exists(normalizedPath) && !watchedDirectories().contains(normalizedPath)) {
            validPaths.append(normalizedPath);
        }
    }

    addPaths(validPaths);
}
