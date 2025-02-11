#include "fileoperations.h"
#include "../../config/constants.h"

#include <QDir>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QFile>

namespace FileOperations {

// Directory and File Management
bool copyDirectoryRecursively(const QString &source, const QString &destination) {
    QDir sourceDir(source);
    if (!sourceDir.exists()) {
        return false;
    }

    QDir destinationDir(destination);
    if (!destinationDir.exists() && !destinationDir.mkpath(".")) {
        return false;
    }

    QFileInfoList entries = sourceDir.entryInfoList(BackupInfo::FILE_SYSTEM_FILTER);
    for (const QFileInfo &entry : entries) {
        QString destPath = destinationDir.filePath(entry.fileName());

        if (entry.isDir()) {
            if (!copyDirectoryRecursively(entry.absoluteFilePath(), destPath)) {
                return false;
            }
        } else if (entry.isFile()) {
            if (!QFile::copy(entry.absoluteFilePath(), destPath)) {
                return false;
            }
        }
    }
    return true;
}

quint64 calculateDirectorySize(const QString &path) {
    quint64 totalSize = 0;
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(BackupInfo::FILE_SYSTEM_FILTER);

    for (const QFileInfo &entry : entries) {
        totalSize += entry.isDir() ? calculateDirectorySize(entry.absoluteFilePath()) : entry.size();
    }
    return totalSize;
}

bool createDirectory(const QString &path) {
    QDir dir(path);
    return dir.exists() || dir.mkpath(".");
}

bool deleteDirectory(const QString &path) {
    QDir dir(path);
    return dir.exists() && dir.removeRecursively();
}

// JSON File Handling
bool writeJsonToFile(const QString &filePath, const QJsonObject &jsonObject) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(jsonObject);
    file.write(doc.toJson());
    file.close();
    return true;
}

QJsonObject readJsonFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QJsonObject();
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc.isObject() ? doc.object() : QJsonObject();
}

// File Collection
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) {
    QDir dir(dirPath);

    // Collect files in the current directory
    QFileInfoList fileEntries = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : fileEntries) {
        QString fullPath = entry.absoluteFilePath();
        if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }

    // Recurse into subdirectories
    QFileInfoList subDirEntries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &subDir : subDirEntries) {
        collectFilesRecursively(subDir.absoluteFilePath(), uniqueFiles, filesArray);
    }
}

// Backup Infrastructure Creation
bool createBackupInfrastructure(const QString &backupDir, QString &errorMessage) {
    QString backupSettingsPath = QDir(backupDir).filePath(AppConfig::BACKUP_SETTINGS_FOLDER);

    // Check if _backup_settings folder exists, if not create it
    if (!QDir().exists(backupSettingsPath)) {
        if (!QDir().mkpath(backupSettingsPath)) {
            errorMessage = QString(AppConfig::ERROR_CREATE_DIR).arg(AppConfig::BACKUP_SETTINGS_FOLDER);
            return false;
        }
    }

    // Check for settings.json inside _backup_settings
    QString settingsFilePath = QDir(backupSettingsPath).filePath(AppConfig::SETTINGS_FILE_NAME);
    if (!QFile::exists(settingsFilePath)) {
        // Create an empty settings.json file
        QFile file(settingsFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            errorMessage = QString(AppConfig::ERROR_CREATE_FILE).arg(AppConfig::SETTINGS_FILE_NAME);
            return false;
        }
        file.close();
    }

    // Check for backup_logs folder inside _backup_settings
    QString backupLogsPath = QDir(backupSettingsPath).filePath(AppConfig::BACKUP_LOGS_FOLDER);
    if (!QDir().exists(backupLogsPath)) {
        if (!QDir().mkpath(backupLogsPath)) {
            errorMessage = QString(AppConfig::ERROR_CREATE_FOLDER).arg(AppConfig::BACKUP_LOGS_FOLDER);
            return false;
        }
    }

    return true; // Everything passed
}

} // namespace FileOperations
