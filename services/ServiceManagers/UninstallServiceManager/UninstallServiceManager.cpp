// Project includes
#include "UninstallServiceManager.h"
#include "UninstallServiceConstants.h"

// Qt includes
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>

// C++ includes

// Forward declaration (Custom class)

// Forward declaration (Qt class)

// Constructor
UninstallServiceManager::UninstallServiceManager() = default;

// Returns the installation directory path
QString UninstallServiceManager::getInstallDirectory() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Prompts user for confirmation and attempts to uninstall the application
bool UninstallServiceManager::promptAndUninstall(QWidget* parent) const {
    const QString installDir = getInstallDirectory();

    if (installDir.isEmpty() || !QDir(installDir).exists()) {
        QMessageBox::warning(parent,
                             UninstallServiceConstants::TITLE_RESET_APP_INSTALLATION,
                             UninstallServiceConstants::MSG_INVALID_DIR);
        return false;
    }

    const QString warningMsg = UninstallServiceConstants::MSG_CONFIRM_RESET.arg(installDir);

    const QMessageBox::StandardButton confirm = QMessageBox::warning(
        parent,
        UninstallServiceConstants::TITLE_CONFIRM_APP_RESET,
        warningMsg,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (confirm != QMessageBox::Yes) {
        return false;
    }

    if (deleteDirectory(installDir)) {
        QMessageBox::information(parent,
                                 UninstallServiceConstants::TITLE_APP_RESET,
                                 UninstallServiceConstants::MSG_SUCCESS);
        return true;
    }

    QMessageBox::critical(parent,
                          UninstallServiceConstants::TITLE_RESET_FAILED,
                          UninstallServiceConstants::MSG_FAILURE);
    return false;
}

// Deletes the directory and all its contents recursively
bool UninstallServiceManager::deleteDirectory(const QString& path) const {
    QDir dir(path);
    return dir.removeRecursively();
}
