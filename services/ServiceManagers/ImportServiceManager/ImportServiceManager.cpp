// Project includes
#include "ImportServiceConstants.h"
#include "ImportServiceManager.h"
#include "../JsonServiceManager/JsonServiceManager.h"
#include "../PathServiceManager/PathServiceManager.h"
#include "../UserServiceManager/UserServiceManager.h"
#include "../../ServiceDirector/ServiceDirector.h"

// Qt includes
#include <QDir>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

// Constructor
ImportServiceManager::ImportServiceManager(QObject* parent)
    : QObject(parent) {}

// Public API
bool ImportServiceManager::importUserPreferences(QWidget* parent) {
    QString filePath = getOpenFilePath(parent);
    if (filePath.isEmpty())
        return false;

    QJsonObject root;
    if (!JsonManager::loadJsonFile(filePath, root) || root.isEmpty()) {
        emit importFailed(ImportServiceConstants::k_IMPORT_ERROR_EMPTY_FILE);
        showWarningDialog(parent,
                          ImportServiceConstants::k_IMPORT_INVALID_FILE_TITLE,
                          ImportServiceConstants::k_IMPORT_INVALID_FILE_MESSAGE);
        return false;
    }

    if (!validateRootObject(root, parent)) {
        emit importFailed(ImportServiceConstants::k_IMPORT_ERROR_MISSING_KEYS);
        return false;
    }

    QJsonObject userSettings  = root.value(ImportServiceConstants::k_JSON_KEY_USER_SETTINGS).toObject();
    QJsonObject userTemplates = root.value(ImportServiceConstants::k_JSON_KEY_USER_TEMPLATES).toObject();

    bool ok1 = JsonManager::saveJsonFile(PathServiceManager::userSettingsFilePath(), userSettings);
    bool ok2 = JsonManager::saveJsonFile(PathServiceManager::userTemplatesFilePath(), userTemplates);

    if (ok1 && ok2) {
        auto* sd = &ServiceDirector::getInstance();
        sd->getUserServiceManager()->load();
        sd->applyTheme();

        emit importSucceeded(filePath);
        showInfoDialog(parent,
                       ImportServiceConstants::k_IMPORT_SUCCESS_TITLE,
                       ImportServiceConstants::k_IMPORT_SUCCESS_MESSAGE);
        return true;
    } else {
        emit importFailed(ImportServiceConstants::k_IMPORT_ERROR_WRITE_FAIL);
        showWarningDialog(parent,
                          ImportServiceConstants::k_IMPORT_FAILURE_TITLE,
                          ImportServiceConstants::k_IMPORT_FAILURE_MESSAGE);
        return false;
    }
}

// Private helpers
QString ImportServiceManager::getOpenFilePath(QWidget* parent) const {
    return QFileDialog::getOpenFileName(
        parent,
        QObject::tr("Import Preferences"),
        QDir::homePath(),
        ImportServiceConstants::k_IMPORT_FILE_FILTER
        );
}

bool ImportServiceManager::validateRootObject(const QJsonObject& root, QWidget* parent) {
    if (!root.contains(ImportServiceConstants::k_JSON_KEY_USER_SETTINGS) ||
        !root.contains(ImportServiceConstants::k_JSON_KEY_USER_TEMPLATES)) {
        showWarningDialog(parent,
                          ImportServiceConstants::k_IMPORT_INVALID_FILE_TITLE,
                          ImportServiceConstants::k_IMPORT_INVALID_FILE_MESSAGE);
        return false;
    }
    return true;
}

void ImportServiceManager::showInfoDialog(QWidget* parent,
                                          const QString& title,
                                          const QString& message) const {
    QMessageBox::information(parent, title, message);
}

void ImportServiceManager::showWarningDialog(QWidget* parent,
                                             const QString& title,
                                             const QString& message) const {
    QMessageBox::warning(parent, title, message);
}
