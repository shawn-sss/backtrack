// Project includes
#include "fileoperations.h"
#include "../../../../constants/backup_config.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QSaveFile>

namespace FileOperations {

// Recursively copies a directory and all its contents
bool copyDirectoryRecursively(const QString& sourcePath, const QString& destinationPath) {
    QDir sourceDir(sourcePath);
    if (!sourceDir.exists()) return false;

    QDir destinationDir(destinationPath);
    if (!destinationDir.exists() && !destinationDir.mkpath(".")) return false;

    const QFileInfoList entries = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo& entry : entries) {
        const QString destPath = destinationDir.filePath(entry.fileName());
        if (entry.isDir()) {
            if (!copyDirectoryRecursively(entry.absoluteFilePath(), destPath)) return false;
        } else {
            if (!QFile::copy(entry.absoluteFilePath(), destPath)) return false;
        }
    }

    return true;
}

// Deletes a directory and all of its contents
bool deleteDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists() && dir.removeRecursively();
}

// Creates a directory if it does not already exist
bool createDirectory(const QString& path) {
    QDir dir(path);
    return dir.exists() || dir.mkpath(".");
}

// Recursively calculates the total size of all files in a directory
quint64 calculateDirectorySize(const QDir& dir) {
    quint64 totalSize = 0;
    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo& entry : entries) {
        totalSize += entry.isDir()
        ? calculateDirectorySize(QDir(entry.absoluteFilePath()))
        : entry.size();
    }

    return totalSize;
}

// Recursively collects all file paths under a directory
void collectFilesRecursively(const QString& dirPath, QSet<QString>& uniqueFiles, QJsonArray& filesArray) {
    QDir dir(dirPath);
    if (!dir.exists()) return;

    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
    for (const QFileInfo& entry : entries) {
        const QString fullPath = entry.absoluteFilePath();
        if (entry.isDir()) {
            collectFilesRecursively(fullPath, uniqueFiles, filesArray);
        } else if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }
}

// Recursively collects all directory paths under a directory
void collectDirectoriesRecursively(const QString& dirPath, QSet<QString>& uniqueFolders, QJsonArray& foldersArray) {
    QDir dir(dirPath);
    if (!dir.exists()) return;

    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    for (const QFileInfo& entry : entries) {
        const QString fullPath = entry.absoluteFilePath();
        if (!uniqueFolders.contains(fullPath)) {
            uniqueFolders.insert(fullPath);
            foldersArray.append(fullPath);
            collectDirectoriesRecursively(fullPath, uniqueFolders, foldersArray);
        }
    }
}

// Creates the initial infrastructure for backups including setup and log folders
bool createBackupInfrastructure(const QString& backupDir, QString& errorMessage) {
    const QDir backupDirectory(backupDir);
    const QString appFolderPath = backupDirectory.filePath(Backup::Storage::k_BACKUP_SETUP_FOLDER);
    const QString logsFolderPath = QDir(appFolderPath).filePath(Backup::Storage::k_BACKUP_LOGS_FOLDER);

    if (!QDir().mkpath(appFolderPath)) {
        errorMessage = QString("Failed to create directory: %1").arg(Backup::Storage::k_BACKUP_SETUP_FOLDER);
        return false;
    }

    if (!QDir().mkpath(logsFolderPath)) {
        errorMessage = QString("Failed to create logs directory: %1").arg(Backup::Storage::k_BACKUP_LOGS_FOLDER);
        return false;
    }

    return true;
}

} // namespace FileOperations
