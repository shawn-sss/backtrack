#include "fileoperations.h"

#include "../../../config/_constants.h"

#include <QDir>
#include <QSet>
#include <QFile>
#include <QSaveFile>
#include <QFileInfoList>
#include <QJsonDocument>

namespace FileOperations {

// Directory copy and deletion
bool copyDirectoryRecursively(const QString &source, const QString &destination) {
    QDir sourceDir(source);
    if (!sourceDir.exists()) return false;

    QDir destinationDir(destination);
    if (!destinationDir.exists() && !destinationDir.mkpath(".")) return false;

    for (const QFileInfo &entry : sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString destPath = destinationDir.filePath(entry.fileName());
        if (entry.isDir()) {
            if (!copyDirectoryRecursively(entry.absoluteFilePath(), destPath)) return false;
        } else if (entry.isFile() && entry.isReadable()) {
            QSaveFile destFile(destPath);
            if (!destFile.open(QIODevice::WriteOnly)) return false;
            QFile sourceFile(entry.absoluteFilePath());
            if (!sourceFile.open(QIODevice::ReadOnly)) return false;
            destFile.write(sourceFile.readAll());
            if (!destFile.commit()) return false;
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

// Directory size calculation
quint64 calculateDirectorySize(const QDir &dir) {
    quint64 totalSize = 0;
    for (const QFileInfo &entry : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
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

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.isObject() ? doc.object() : QJsonObject();
}

// Recursive file and folder collection
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) {
    QDir dir(dirPath);
    if (!dir.exists()) return;

    for (const QFileInfo &entry : dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
        QString fullPath = entry.absoluteFilePath();
        if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }

    for (const QFileInfo &subDir : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        collectFilesRecursively(subDir.absoluteFilePath(), uniqueFiles, filesArray);
    }
}

void collectDirectoriesRecursively(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) {
    QDir dir(dirPath);
    if (!dir.exists()) return;

    for (const QFileInfo &subDir : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
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
    QDir backupRoot(backupDir);
    if (!backupRoot.exists() && !backupRoot.mkpath(".")) {
        errorMessage = QString(ErrorMessages::ERROR_CREATE_BACKUP_DIRECTORY).arg(AppConfig::BACKUP_CONFIG_FOLDER);
        return false;
    }

    QString backupSettingsPath = backupRoot.filePath(AppConfig::BACKUP_CONFIG_FOLDER);
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
        errorMessage = QString(ErrorMessages::ERROR_CREATE_BACKUP_DIRECTORY).arg(AppConfig::BACKUP_LOGS_DIRECTORY);
        return false;
    }

    return true;
}

} // namespace FileOperations
