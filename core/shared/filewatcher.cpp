// Project includes
#include "filewatcher.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QSet>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>

// C++ includes
#include <algorithm>

// Normalize and validate path
namespace {
QString normalizeAndCheckPath(const QString& path, bool allowNonexistentDir) {
    QString normalized = QDir::fromNativeSeparators(path.trimmed());
    QFileInfo info(normalized);

    if (info.exists()) return normalized;
    if (allowNonexistentDir && info.isDir()) return normalized;

    return {};
}
}

// Constructor and signals
FileWatcher::FileWatcher(QObject* parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Update the entire watch list
void FileWatcher::updateWatchList(const QStringList& paths) {
    removeAllPaths();
    addPaths(paths);
}

// Add a single path
void FileWatcher::addPath(const QString& path) {
    const QString normalized = normalizeAndCheckPath(path, true);
    if (normalized.isEmpty()) return;

    if (!watcher->directories().contains(normalized) && !watcher->files().contains(normalized)) {
        watcher->addPath(normalized);
    }
}

// Add multiple paths
void FileWatcher::addPaths(const QStringList& paths) {
    QSet<QString> currentWatched;

    const QStringList& dirs = watcher->directories();
    for (int i = 0; i < dirs.size(); ++i)
        currentWatched.insert(dirs.at(i));

    const QStringList& files = watcher->files();
    for (int i = 0; i < files.size(); ++i)
        currentWatched.insert(files.at(i));

    QStringList newPaths;
    for (int i = 0; i < paths.size(); ++i) {
        QString normalized = QDir::fromNativeSeparators(paths.at(i).trimmed());
        if (!normalized.isEmpty() && !currentWatched.contains(normalized)) {
            newPaths.append(normalized);
        }
    }

    if (!newPaths.isEmpty()) {
        watcher->addPaths(newPaths);
    }
}

// Remove a single path
void FileWatcher::removePath(const QString& path) {
    const QString normalized = QDir::fromNativeSeparators(path.trimmed());
    if (watcher->directories().contains(normalized) || watcher->files().contains(normalized)) {
        watcher->removePath(normalized);
    }
}

// Remove all paths
void FileWatcher::removeAllPaths() {
    const QStringList allPaths = watcher->directories() + watcher->files();
    if (!allPaths.isEmpty()) {
        watcher->removePaths(allPaths);
    }
}

// Return watched directories
QStringList FileWatcher::watchedDirectories() const {
    return watcher->directories();
}

// Return watched files
QStringList FileWatcher::watchedFiles() const {
    return watcher->files();
}

// Start watching special folders
void FileWatcher::startWatchingMultiple(const QStringList& roots) {
    static QStringList lastWatched;

    QStringList sortedRoots = roots;
    std::sort(sortedRoots.begin(), sortedRoots.end());
    if (sortedRoots == lastWatched) return;

    lastWatched = sortedRoots;
    removeAllPaths();

    QSet<QString> pathsToWatch;

    const QString appConfigFolder = PathServiceManager::appConfigFolderPath();
    const QString backupRoot = PathServiceManager::backupSetupFolderPath();
    const QString backupConfigFolder = PathServiceManager::backupConfigFolderPath();
    const QString backupInit = PathServiceManager::backupInitMetadataFilePath();
    const QString logsFolder = PathServiceManager::backupLogsFolderPath();

    for (int i = 0; i < sortedRoots.size(); ++i) {
        const QString& root = sortedRoots.at(i);
        const QString normalizedRoot = QDir::cleanPath(root);
        if (normalizedRoot.isEmpty()) continue;

        pathsToWatch.insert(normalizedRoot);

        if (normalizedRoot == appConfigFolder) {
            const QStringList appFiles = {
                QDir::cleanPath(appConfigFolder + "/app_init.json"),
                QDir::cleanPath(appConfigFolder + "/app_notifications.json"),
                QDir::cleanPath(appConfigFolder + "/user_settings.json")
            };
            for (int j = 0; j < appFiles.size(); ++j) {
                pathsToWatch.insert(appFiles.at(j));
            }
        }

        if (normalizedRoot == backupRoot) {
            pathsToWatch.insert(QDir::cleanPath(backupInit));
            pathsToWatch.insert(QDir::cleanPath(logsFolder));

            QDir logDir(logsFolder);
            const QFileInfoList logFiles = logDir.entryInfoList(QStringList() << "*.json", QDir::Files);
            for (const QFileInfo& fileInfo : logFiles) {
                const QString filePath = fileInfo.absoluteFilePath();
                if (!filePath.isEmpty()) {
                    pathsToWatch.insert(filePath);
                }
            }
        }
    }

    addPaths(pathsToWatch.values());
}
