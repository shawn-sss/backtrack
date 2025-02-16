#include "fileoperations.h"
#include "../../config/_constants.h"

#include <QDir>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QFile>
#include <QSet>
#include <QSaveFile>

namespace FileOperations {

// Directory and file management
bool copyDirectoryRecursively(const QString &source, const QString &destination) {
    QDir sourceDir(source);
    if (!sourceDir.exists()) return false;

    QDir destinationDir(destination);
    if (!destinationDir.exists() && !destinationDir.mkpath(".")) return false;

    const QFileInfoList entries = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    for (int i = 0; i < entries.size(); ++i) {
        const QFileInfo &entry = entries.at(i);
        QString destPath = destinationDir.filePath(entry.fileName());

        if (entry.isDir()) {
            if (!copyDirectoryRecursively(entry.absoluteFilePath(), destPath)) return false;
        } else if (entry.isFile()) {
            if (QFile::exists(destPath)) QFile::remove(destPath);
            if (!QFile::copy(entry.absoluteFilePath(), destPath)) return false;
        }
    }
    return true;
}

bool deleteDirectory(const QString &path) {
    QDir dir(path);
    return dir.exists() && dir.removeRecursively();
}

bool createDirectory(const QString &path) {
    QDir dir(path);
    return dir.exists() || dir.mkpath(".");
}

// File size calculation
quint64 calculateDirectorySize(const QDir &dir) {
    quint64 totalSize = 0;
    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    for (int i = 0; i < entries.size(); ++i) {
        const QFileInfo &entry = entries.at(i);
        totalSize += entry.isDir() ? calculateDirectorySize(QDir(entry.absoluteFilePath())) : entry.size();
    }
    return totalSize;
}


// JSON file handling
bool writeJsonToFile(const QString &filePath, const QJsonObject &jsonObject) {
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QJsonDocument doc(jsonObject);
    if (file.write(doc.toJson()) == -1) return false;

    return file.commit();
}

QJsonObject readJsonFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return QJsonObject();

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc.isObject() ? doc.object() : QJsonObject();
}

// Recursive Traversal Methods
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) {
    QDir dir(dirPath);
    const QFileInfoList fileEntries = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    for (int i = 0; i < fileEntries.size(); ++i) {
        const QFileInfo &entry = fileEntries.at(i);
        QString fullPath = entry.absoluteFilePath();
        if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }

    const QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (int i = 0; i < subDirs.size(); ++i) {
        const QFileInfo &subDir = subDirs.at(i);
        collectFilesRecursively(subDir.absoluteFilePath(), uniqueFiles, filesArray);
    }
}

void collectDirectoriesRecursively(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) {
    QDir dir(dirPath);
    const QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (int i = 0; i < subDirs.size(); ++i) {
        const QFileInfo &subDir = subDirs.at(i);
        QString subDirPath = subDir.absoluteFilePath();
        if (!uniqueFolders.contains(subDirPath)) {
            uniqueFolders.insert(subDirPath);
            foldersArray.append(subDirPath);
            collectDirectoriesRecursively(subDirPath, uniqueFolders, foldersArray);
        }
    }
}

// Backup infrastructure setup
bool createBackupInfrastructure(const QString &backupDir, QString &errorMessage) {
    QString backupSettingsPath = QDir(backupDir).filePath(AppConfig::BACKUP_CONFIG_FOLDER);

    QDir settingsDir(backupSettingsPath);
    if (!settingsDir.exists() && !settingsDir.mkpath(".")) {
        errorMessage = QString(ErrorMessages::ERROR_CREATE_BACKUP_DIRECTORY).arg(AppConfig::BACKUP_CONFIG_FOLDER);
        return false;
    }

    QString settingsFilePath = settingsDir.filePath(AppConfig::CONFIG_FILE_NAME);
    if (!QFile::exists(settingsFilePath)) {
        QFile file(settingsFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            errorMessage = QString(ErrorMessages::ERROR_CREATE_BACKUP_FILE).arg(AppConfig::CONFIG_FILE_NAME);
            return false;
        }
        file.close();
    }

    QString backupLogsPath = settingsDir.filePath(AppConfig::BACKUP_LOGS_DIRECTORY);
    QDir logsDir(backupLogsPath);
    if (!logsDir.exists() && !logsDir.mkpath(".")) {
        errorMessage = QString(ErrorMessages::ERROR_CREATE_BACKUP_FOLDER).arg(AppConfig::BACKUP_LOGS_DIRECTORY);
        return false;
    }

    return true;
}

} // namespace FileOperations
