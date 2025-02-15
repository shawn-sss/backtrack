#include "filewatcher.h"
#include "../../config/constants.h"

#include <QDir>
#include <QFileInfo>

// Constructor & Initialization
FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Path Management
void FileWatcher::addPath(const QString &path) {
    if (!(watcher->directories() + watcher->files()).contains(path)) {
        watcher->addPath(path);
    }
}

void FileWatcher::removePath(const QString &path) {
    if ((watcher->directories() + watcher->files()).contains(path)) {
        watcher->removePath(path);
    }
}

void FileWatcher::addPaths(const QStringList &paths) {
    watcher->addPaths(paths);
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
    addPath(rootPath);

    QDir rootDir(rootPath);
    const QFileInfoList subDirectories = rootDir.entryInfoList(BackupInfo::FILE_SYSTEM_FILTER);

    for (int i = 0; i < subDirectories.size(); ++i) {
        const QFileInfo &dirInfo = subDirectories.at(i);
        QFileInfo summaryFile(QDir(dirInfo.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME));
        if (summaryFile.exists()) {
            addPath(summaryFile.absoluteFilePath());
        }
    }
}
