// Project includes
#include "FilewatcherServiceManager.h"
#include "../../../constants/app_info.h"
#include "../PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QSet>

// C++ includes
#include <algorithm>

// Required application config files
namespace AppConfigFiles {
inline const QStringList& Required() {
    static const QStringList files = {
        App::Items::k_APPDATA_SETUP_INFO_FILE,
        App::Items::k_APPDATA_SETUP_NOTIFICATIONS_FILE,
        App::Items::k_APPDATA_SETUP_USER_SETTINGS_FILE
    };
    return files;
}
}

namespace {

QString normalizeAndCheckPath(const QString& path, bool allowNonexistentDir) {
    const QString normalized = QDir::fromNativeSeparators(path.trimmed());
    QFileInfo info(normalized);
    return (info.exists() || (allowNonexistentDir && info.isDir())) ? normalized : QString{};
}

QStringList buildCompleteWatchList(const QStringList& roots) {
    QSet<QString> pathsToWatch;
    const QString appConfigFolder = PathServiceManager::appConfigFolderPath();
    const QString backupRoot = PathServiceManager::backupConfigFolderPath();
    const QString backupInit = PathServiceManager::backupInitMetadataFilePath();
    const QString logsFolder = PathServiceManager::backupLogsFolderPath();

    for (const QString& root : roots) {
        const QString normalizedRoot = QDir::cleanPath(root);
        if (normalizedRoot.isEmpty()) continue;

        pathsToWatch.insert(normalizedRoot);

        if (normalizedRoot == QDir::cleanPath(backupRoot)) {
            pathsToWatch.unite({
                QDir::cleanPath(backupInit),
                QDir::cleanPath(logsFolder),
                QDir::cleanPath(backupRoot)
            });

            QDir logDir(logsFolder);
            for (const QFileInfo& fileInfo : logDir.entryInfoList({"*.json"}, QDir::Files)) {
                pathsToWatch.insert(fileInfo.absoluteFilePath());
            }
        }
    }

    for (const QString& file : AppConfigFiles::Required()) {
        pathsToWatch.insert(QDir::cleanPath(QDir(appConfigFolder).filePath(file)));
    }

    return pathsToWatch.values();
}

} // namespace

// Construct file watcher and connect signals
FileWatcher::FileWatcher(QObject* parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this)) {
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::directoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::fileChanged);
}

// Replace current watch list with new paths
void FileWatcher::updateWatchList(const QStringList& paths) {
    removeAllPaths();
    addPaths(paths);
}

// Add single path to watcher
void FileWatcher::addPath(const QString& path) {
    const QString normalized = normalizeAndCheckPath(path, true);
    if (!normalized.isEmpty() &&
        !watcher->directories().contains(normalized) &&
        !watcher->files().contains(normalized)) {
        watcher->addPath(normalized);
    }
}

// Add multiple new paths to watcher
void FileWatcher::addPaths(const QStringList& paths) {
    QSet<QString> currentWatched;
    currentWatched.unite(QSet<QString>(watcher->directories().begin(), watcher->directories().end()));
    currentWatched.unite(QSet<QString>(watcher->files().begin(), watcher->files().end()));

    QStringList newPaths;
    for (const QString& path : paths) {
        const QString normalized = QDir::fromNativeSeparators(path.trimmed());
        if (!normalized.isEmpty() && !currentWatched.contains(normalized)) {
            newPaths.append(normalized);
        }
    }

    if (!newPaths.isEmpty()) {
        watcher->addPaths(newPaths);
    }
}

// Remove a single path from watcher
void FileWatcher::removePath(const QString& path) {
    const QString normalized = QDir::fromNativeSeparators(path.trimmed());
    if (watcher->directories().contains(normalized) || watcher->files().contains(normalized)) {
        watcher->removePath(normalized);
    }
}

// Remove all currently watched paths
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

// Watch root directories along with related files
void FileWatcher::startWatchingMultiple(const QStringList& roots) {
    static QStringList lastWatched;

    QStringList sortedRoots = roots;
    std::sort(sortedRoots.begin(), sortedRoots.end());

    const QStringList currentPathsToWatch = buildCompleteWatchList(roots);
    const QSet<QString> newPathSet(currentPathsToWatch.begin(), currentPathsToWatch.end());

    const QStringList dirs = watcher->directories();
    const QStringList files = watcher->files();

    QSet<QString> currentWatchedSet;
    currentWatchedSet.unite(QSet<QString>(dirs.begin(), dirs.end()));
    currentWatchedSet.unite(QSet<QString>(files.begin(), files.end()));

    if (sortedRoots == lastWatched && newPathSet == currentWatchedSet)
        return;

    lastWatched = sortedRoots;
    removeAllPaths();
    addPaths(currentPathsToWatch);
}
