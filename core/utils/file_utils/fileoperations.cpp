// Project includes same directory
#include "fileoperations.h"

// Project includes different directory
#include "../../../config/_constants.h"

// Built-in Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QSaveFile>

namespace FileOperations {

// Copies a directory and its contents recursively
bool copyDirectoryRecursively(const QString &sourcePath, const QString &destinationPath) {
    QDir sourceDir(sourcePath);
    if (!sourceDir.exists()) return false;

    QDir destinationDir(destinationPath);
    if (!destinationDir.exists() && !destinationDir.mkpath(".")) return false;

    const QFileInfoList entries = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo &entry : entries) {
        const QString destPath = destinationDir.filePath(entry.fileName());
        if (entry.isDir()) {
            if (!copyDirectoryRecursively(entry.absoluteFilePath(), destPath)) return false;
        } else {
            if (!QFile::copy(entry.absoluteFilePath(), destPath)) return false;
        }
    }
    return true;
}

// Deletes a directory and its contents
bool deleteDirectory(const QString &path) {
    QDir dir(path);
    return dir.exists() && dir.removeRecursively();
}

// Creates a directory if it does not exist
bool createDirectory(const QString &path) {
    QDir dir(path);
    return dir.exists() || dir.mkpath(".");
}

// Calculates the total size of a directory
quint64 calculateDirectorySize(const QDir &dir) {
    quint64 totalSize = 0;
    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo &entry : entries) {
        totalSize += entry.isDir() ? calculateDirectorySize(QDir(entry.absoluteFilePath())) : entry.size();
    }
    return totalSize;
}

// Writes a JSON object to a file
bool writeJsonToFile(const QString &filePath, const QJsonObject &jsonObject) {
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QJsonDocument doc(jsonObject);
    if (file.write(doc.toJson()) == -1) return false;

    return file.commit();
}

// Reads a JSON object from a file
QJsonObject readJsonFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return QJsonObject();

    const QByteArray data = file.readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(data);

    return doc.isObject() ? doc.object() : QJsonObject();
}

// Collects all files within a directory recursively
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) {
    QDir dir(dirPath);
    if (!dir.exists()) return;

    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
    for (const QFileInfo &entry : entries) {
        const QString fullPath = entry.absoluteFilePath();
        if (entry.isDir()) {
            collectFilesRecursively(fullPath, uniqueFiles, filesArray);
        } else if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }
}

// Collects all directories within a directory recursively
void collectDirectoriesRecursively(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) {
    QDir dir(dirPath);
    if (!dir.exists()) return;

    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    for (const QFileInfo &entry : entries) {
        const QString fullPath = entry.absoluteFilePath();
        if (!uniqueFolders.contains(fullPath)) {
            uniqueFolders.insert(fullPath);
            foldersArray.append(fullPath);
            collectDirectoriesRecursively(fullPath, uniqueFolders, foldersArray);
        }
    }
}

// Creates the required directory structure for backups
bool createBackupInfrastructure(const QString &backupDir, QString &errorMessage) {
    const QDir backupDirectory(backupDir);
    const QString appFolderPath = backupDirectory.filePath(AppConfig::BACKUP_CONFIG_FOLDER);
    const QString logsFolderPath = QDir(appFolderPath).filePath(AppConfig::BACKUP_LOGS_DIRECTORY);

    if (!QDir().mkpath(appFolderPath)) {
        errorMessage = QString("Failed to create directory: %1").arg(AppConfig::BACKUP_CONFIG_FOLDER);
        return false;
    }

    if (!QDir().mkpath(logsFolderPath)) {
        errorMessage = QString("Failed to create logs directory: %1").arg(AppConfig::BACKUP_LOGS_DIRECTORY);
        return false;
    }

    return true;
}

} // namespace FileOperations
