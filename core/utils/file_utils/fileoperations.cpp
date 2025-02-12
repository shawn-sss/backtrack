#include "fileoperations.h"
#include "../../config/constants.h"

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

    QFileInfoList entries = sourceDir.entryInfoList(BackupInfo::FILE_SYSTEM_FILTER);
    for (const QFileInfo &entry : entries) {
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
    QFileInfoList entries = dir.entryInfoList(BackupInfo::FILE_SYSTEM_FILTER);

    for (const QFileInfo &entry : entries) {
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

// File collection operations
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) {
    QDir dir(dirPath);
    QFileInfoList fileEntries = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    for (const QFileInfo &entry : fileEntries) {
        QString fullPath = entry.absoluteFilePath();
        if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }

    QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &subDir : subDirs) {
        collectFilesRecursively(subDir.absoluteFilePath(), uniqueFiles, filesArray);
    }
}

// Backup infrastructure setup
bool createBackupInfrastructure(const QString &backupDir, QString &errorMessage) {
    QString backupSettingsPath = QDir(backupDir).filePath(AppConfig::BACKUP_SETTINGS_FOLDER);

    QDir settingsDir(backupSettingsPath);
    if (!settingsDir.exists() && !settingsDir.mkpath(".")) {
        errorMessage = QString(AppConfig::ERROR_CREATE_DIR).arg(AppConfig::BACKUP_SETTINGS_FOLDER);
        return false;
    }

    QString settingsFilePath = settingsDir.filePath(AppConfig::SETTINGS_FILE_NAME);
    if (!QFile::exists(settingsFilePath)) {
        QFile file(settingsFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            errorMessage = QString(AppConfig::ERROR_CREATE_FILE).arg(AppConfig::SETTINGS_FILE_NAME);
            return false;
        }
        file.close();
    }

    QString backupLogsPath = settingsDir.filePath(AppConfig::BACKUP_LOGS_FOLDER);
    QDir logsDir(backupLogsPath);
    if (!logsDir.exists() && !logsDir.mkpath(".")) {
        errorMessage = QString(AppConfig::ERROR_CREATE_FOLDER).arg(AppConfig::BACKUP_LOGS_FOLDER);
        return false;
    }

    return true;
}

} // namespace FileOperations
