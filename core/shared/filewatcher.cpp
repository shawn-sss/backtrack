// Project includes
#include "filewatcher.h"
#include "../../../../constants/backup_config.h"

// Qt includes
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QSet>

// Initializes the file watcher and connects signals
FileWatcher::FileWatcher(QObject* parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Replaces all currently watched paths with new ones
void FileWatcher::updateWatchList(const QStringList& paths) {
    removeAllPaths();
    addPaths(paths);
}

// Adds a single file or directory to the watcher
void FileWatcher::addPath(const QString& path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (!normalizedPath.isEmpty() &&
        !watcher->directories().contains(normalizedPath) &&
        !watcher->files().contains(normalizedPath) &&
        QFileInfo::exists(normalizedPath)) {
        watcher->addPath(normalizedPath);
    }
}

// Adds multiple files or directories to the watcher
void FileWatcher::addPaths(const QStringList& paths) {
    QStringList newPaths;

    const QStringList dirs = watcher->directories();
    const QStringList files = watcher->files();

    QSet<QString> currentWatched(dirs.begin(), dirs.end());
    currentWatched.unite(QSet<QString>(files.begin(), files.end()));

    for (const QString& path : paths) {
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

// Removes a single file or directory from the watcher
void FileWatcher::removePath(const QString& path) {
    QString normalizedPath = QDir::fromNativeSeparators(path);
    if (watcher->directories().contains(normalizedPath) ||
        watcher->files().contains(normalizedPath)) {
        watcher->removePath(normalizedPath);
    }
}

// Removes all currently watched paths
void FileWatcher::removeAllPaths() {
    QStringList allPaths = watcher->directories() + watcher->files();
    if (!allPaths.isEmpty()) {
        watcher->removePaths(allPaths);
    }
}

// Returns list of directories currently being watched
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Returns list of files currently being watched
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Starts watching a backup root folder and its structure
void FileWatcher::startWatching(const QString& rootPath) {
    removeAllPaths();

    QDir rootDir(rootPath);
    if (!rootDir.exists()) return;

    QSet<QString> pathsToWatch;

    const QString setupFolder = rootDir.filePath(Backup::Storage::k_BACKUP_SETUP_FOLDER);
    const QString setupInfoFile = rootDir.filePath(
        QString(Backup::Storage::k_BACKUP_SETUP_FOLDER) + "/" + Backup::Storage::k_BACKUP_SETUP_INFO_FILE);
    const QString logsFolder = rootDir.filePath(
        QString(Backup::Storage::k_BACKUP_SETUP_FOLDER) + "/" + Backup::Storage::k_BACKUP_LOGS_FOLDER);

    pathsToWatch.insert(rootPath);
    pathsToWatch.insert(setupFolder);
    pathsToWatch.insert(setupInfoFile);
    pathsToWatch.insert(logsFolder);

    QDir logsDir(logsFolder);
    const QFileInfoList logFiles = logsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QFileInfo& logFile : logFiles) {
        pathsToWatch.insert(logFile.absoluteFilePath());
    }

    const QFileInfoList subDirs = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& dirInfo : subDirs) {
        pathsToWatch.insert(dirInfo.absoluteFilePath());
    }

    QStringList validPaths;
    for (const QString& path : std::as_const(pathsToWatch)) {
        if (QFileInfo::exists(path)) {
            validPaths << QDir::fromNativeSeparators(path);
        }
    }

    addPaths(validPaths);
}
