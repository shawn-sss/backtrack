#include "UninstallServiceManager.h"
#include "UninstallServiceConstants.h"

// Qt includes
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>

using namespace UninstallServiceConstants;

// Constructs the uninstall service manager
UninstallServiceManager::UninstallServiceManager() = default;

// Returns the app's local installation directory
QString UninstallServiceManager::getInstallDirectory() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Prompts the user to confirm uninstall
bool UninstallServiceManager::confirmUninstall(QWidget* parent) const {
    const QString installDir = getInstallDirectory();

    if (installDir.isEmpty() || !QDir(installDir).exists()) {
        QMessageBox::warning(parent, titleResetAppInstallation(), msgInvalidDir());
        return false;
    }

    const QMessageBox::StandardButton confirm = QMessageBox::warning(
        parent,
        titleConfirmAppReset(),
        msgConfirmReset(installDir),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    return (confirm == QMessageBox::Yes);
}

// Performs the uninstall and notifies the user
bool UninstallServiceManager::performUninstall() const {
    const QString installDir = getInstallDirectory();

    if (deleteDirectory(installDir)) {
        QMessageBox::information(nullptr, titleAppReset(), msgSuccess());
        return true;
    }

    QMessageBox::critical(nullptr, titleResetFailed(), msgFailure());
    return false;
}

// Deletes the specified directory recursively
bool UninstallServiceManager::deleteDirectory(const QString& path) const {
    return QDir(path).removeRecursively();
}
