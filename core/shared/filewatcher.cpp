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
    for (const QString& dir : watcher->directories())
        currentWatched.insert(dir);
    for (const QString& file : watcher->files())
        currentWatched.insert(file);

    QStringList newPaths;
    for (const QString& path : paths) {
        QString normalized = QDir::fromNativeSeparators(path.trimmed());
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
    const QString backupRoot = PathServiceManager::backupSetupFolderPath();
    const QString backupConfigFolder = PathServiceManager::backupConfigFolderPath();
    const QString backupInit = PathServiceManager::backupInitMetadataFilePath();
    const QString logsFolder = PathServiceManager::backupLogsFolderPath();

    for (const QString& root : sortedRoots) {
        const QString normalizedRoot = QDir::cleanPath(root);
        if (normalizedRoot.isEmpty()) continue;

        pathsToWatch.insert(normalizedRoot);

        if (normalizedRoot.endsWith("app_config")) {
            const QStringList appFiles = {
                normalizedRoot + "/app_init.json",
                normalizedRoot + "/app_notifications.json",
                normalizedRoot + "/user_settings.json"
            };
            for (const QString& file : appFiles) {
                pathsToWatch.insert(QDir::cleanPath(file));
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
