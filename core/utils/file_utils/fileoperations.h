#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QSet>
#include <QDir>

// File operations utilities
namespace FileOperations {

// Directory and File Management
bool copyDirectoryRecursively(const QString &source, const QString &destination);
bool createDirectory(const QString &path);
bool deleteDirectory(const QString &path);

// File Size Calculation
quint64 calculateDirectorySize(const QDir &dir);

// JSON File Handling
bool writeJsonToFile(const QString &filePath, const QJsonObject &jsonObject);
QJsonObject readJsonFromFile(const QString &filePath);

// File and Directory Collection
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray);
void collectDirectoriesRecursively(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray);

// Backup Infrastructure Setup
bool createBackupInfrastructure(const QString &backupDir, QString &errorMessage);

} // namespace FileOperations

#endif // FILEOPERATIONS_H
