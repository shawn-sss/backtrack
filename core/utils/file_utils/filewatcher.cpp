#include "filewatcher.h"
#include "../../config/_constants.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <utility>

// Constructor & Initialization
FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Updates the watch list dynamically
void FileWatcher::updateWatchList(const QStringList &paths) {
    watchList = paths;
}

// Path Management
void FileWatcher::addPath(const QString &path) {
    if (!path.isEmpty() && !(watcher->directories() + watcher->files()).contains(path)) {
        watcher->addPath(path);
        qDebug() << "Watching path: " << path;
    }
}

void FileWatcher::addPaths(const QStringList &paths) {
    for (const QString &path : paths) {
        addPath(path);
    }
}

void FileWatcher::removePath(const QString &path) {
    if ((watcher->directories() + watcher->files()).contains(path)) {
        watcher->removePath(path);
        qDebug() << "Stopped watching path: " << path;
    }
}

void FileWatcher::removeAllPaths() {
    if (!watchedDirectories().isEmpty()) {
        watcher->removePaths(watcher->directories());
    }
    if (!watchedFiles().isEmpty()) {
        watcher->removePaths(watcher->files());
    }
}

// Retrieval Methods
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Monitoring Functionality
void FileWatcher::startWatching(const QString &rootPath) {
    removeAllPaths();
    addPath(rootPath);  // Always watch the root backup directory

    QDir rootDir(rootPath);
    const QFileInfoList subDirectories = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Define important paths to watch dynamically
    watchList = {
        QDir(rootPath).filePath(AppConfig::BACKUP_CONFIG_FOLDER),
        QDir(rootPath).filePath(AppConfig::BACKUP_LOGS_DIRECTORY),
        QDir(rootPath).filePath(AppConfig::CONFIG_FILE_NAME)
    };

    // Add all subdirectories to the watch list
    for (const QFileInfo &dirInfo : subDirectories) {
        watchList.append(dirInfo.absoluteFilePath());
    }

    // Ensure the paths actually exist before adding them to the watcher
    QStringList validPaths;
    for (const auto &path : std::as_const(watchList)) {  // Use std::as_const to prevent detachment
        if (QFileInfo::exists(path)) {  // Use static QFileInfo::exists() method
            validPaths.append(path);
        }
    }

    // Start watching the valid paths
    addPaths(validPaths);

    // Debug output
    qDebug() << "Currently watching paths: " << watchedDirectories() + watchedFiles();
}
