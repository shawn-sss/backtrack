#include "filewatcher.h"
#include "../Utils/constants.h"

#include <QDir>
#include <QFileInfo>

// Constructor
FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Path Management

// Add a single path to the watcher
void FileWatcher::addPath(const QString &path) {
    if (!watcher->directories().contains(path) && !watcher->files().contains(path)) {
        watcher->addPath(path);
    }
}

// Remove a single path from the watcher
void FileWatcher::removePath(const QString &path) {
    if (watcher->directories().contains(path) || watcher->files().contains(path)) {
        watcher->removePath(path);
    }
}

// Add multiple paths to the watcher
void FileWatcher::addPaths(const QStringList &paths) {
    for (const QString &path : paths) {
        addPath(path);
    }
}

// Remove all watched paths
void FileWatcher::removeAllPaths() {
    if (!watchedDirectories().isEmpty()) {
        watcher->removePaths(watcher->directories());
    }
    if (!watchedFiles().isEmpty()) {
        watcher->removePaths(watcher->files());
    }
}

// Retrieval Methods

// Get the list of currently watched directories
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Get the list of currently watched files
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Monitoring Functionality

// Start watching a root directory and its relevant subdirectories
void FileWatcher::startWatching(const QString &rootPath) {
    removeAllPaths(); // Clear all previously watched paths
    addPath(rootPath); // Add the root directory to watch

    // Add subdirectories and relevant files within the root path
    QDir rootDir(rootPath);
    QFileInfoList subDirectories = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Use indexed access to avoid range-loop detach warning
    for (int i = 0; i < subDirectories.size(); ++i) {
        const QFileInfo &dirInfo = subDirectories.at(i); // Safe indexed access
        QString summaryFilePath = QDir(dirInfo.absoluteFilePath()).filePath(Constants::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(summaryFilePath)) {
            addPath(summaryFilePath); // Watch the summary file path
        }
    }
}
