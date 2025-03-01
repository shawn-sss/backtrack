#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QString>
#include <QDir>
#include <QSet>
#include <QJsonObject>
#include <QJsonArray>

// FileOperations namespace - file and directory utilities
namespace FileOperations {

// Directory and file management
bool copyDirectoryRecursively(const QString &source, const QString &destination);
bool createDirectory(const QString &path);
bool deleteDirectory(const QString &path);

// File size calculation
quint64 calculateDirectorySize(const QDir &dir);

// JSON file handling
bool writeJsonToFile(const QString &filePath, const QJsonObject &jsonObject);
QJsonObject readJsonFromFile(const QString &filePath);

// Recursive file and directory collection
void collectFilesRecursively(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray);
void collectDirectoriesRecursively(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray);

// Backup infrastructure setup
bool createBackupInfrastructure(const QString &backupDir, QString &errorMessage);

} // namespace FileOperations

#endif // FILEOPERATIONS_H
