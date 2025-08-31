// Project includes
#include "ExportServiceConstants.h"
#include "ExportServiceManager.h"
#include "../JsonServiceManager/JsonServiceManager.h"
#include "../PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QDir>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

// Constructor
ExportServiceManager::ExportServiceManager(QObject* parent)
    : QObject(parent) {}

// Public API
bool ExportServiceManager::exportUserPreferences(QWidget* parent) {
    QString filePath = getSaveFilePath(parent);
    if (filePath.isEmpty())
        return false;

    QJsonObject root = createExportObject();
    bool success = JsonManager::saveJsonFile(filePath, root);

    if (success) {
        emit exportSucceeded(filePath);
        showInfoDialog(parent,
                       ExportServiceConstants::k_EXPORT_SUCCESS_TITLE,
                       ExportServiceConstants::k_EXPORT_SUCCESS_MESSAGE);
        return true;
    } else {
        emit exportFailed(ExportServiceConstants::k_EXPORT_FAILURE_GENERIC);
        showWarningDialog(parent,
                          ExportServiceConstants::k_EXPORT_FAILURE_TITLE,
                          ExportServiceConstants::k_EXPORT_FAILURE_MESSAGE);
        return false;
    }
}

// Private helpers
QString ExportServiceManager::getSaveFilePath(QWidget* parent) const {
    return QFileDialog::getSaveFileName(
        parent,
        QObject::tr("Export Preferences"),
        QDir::homePath() + "/" + ExportServiceConstants::k_EXPORT_DEFAULT_FILENAME,
        ExportServiceConstants::k_EXPORT_FILE_FILTER
        );
}

QJsonObject ExportServiceManager::createExportObject() const {
    struct ExportSection {
        QString key;
        QString path;
    };

    QList<ExportSection> sections = {
        { ExportServiceConstants::k_JSON_KEY_USER_SETTINGS,  PathServiceManager::userSettingsFilePath() },
        { ExportServiceConstants::k_JSON_KEY_USER_TEMPLATES, PathServiceManager::userTemplatesFilePath() }
    };

    QJsonObject root;
    for (const auto& section : sections) {
        QJsonObject obj;
        JsonManager::loadJsonFile(section.path, obj);
        root[section.key] = obj;
    }

    return root;
}

void ExportServiceManager::showInfoDialog(QWidget* parent,
                                          const QString& title,
                                          const QString& message) const {
    QMessageBox::information(parent, title, message);
}

void ExportServiceManager::showWarningDialog(QWidget* parent,
                                             const QString& title,
                                             const QString& message) const {
    QMessageBox::warning(parent, title, message);
}
