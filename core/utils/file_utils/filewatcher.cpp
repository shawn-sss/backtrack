#include "filewatcher.h"
#include "../../config/constants.h"

#include <QDir>
#include <QFileInfo>

// Constructor
FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Path Management
void FileWatcher::addPath(const QString &path) {
    if (!watcher->directories().contains(path) && !watcher->files().contains(path)) {
        watcher->addPath(path);
    }
}

void FileWatcher::removePath(const QString &path) {
    if (watcher->directories().contains(path) || watcher->files().contains(path)) {
        watcher->removePath(path);
    }
}

void FileWatcher::addPaths(const QStringList &paths) {
    for (const QString &path : paths) {
        addPath(path);
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
    addPath(rootPath);

    QDir rootDir(rootPath);
    QFileInfoList subDirectories = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &dirInfo : subDirectories) {
        QString summaryFilePath = QDir(dirInfo.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(summaryFilePath)) {
            addPath(summaryFilePath);
        }
    }
}
