// Project includes
#include "UninstallServiceManager.h"
#include "UninstallServiceConstants.h"
#include "../../../ui/promptdialog/promptdialog.h"

// Qt includes
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>

using namespace UninstallServiceConstants;

UninstallServiceManager::UninstallServiceManager() = default;

// Returns the app's local installation directory
QString UninstallServiceManager::getInstallDirectory() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Prompts the user to confirm uninstall
bool UninstallServiceManager::confirmUninstall(QWidget* parent) const {
    const QString installDir = getInstallDirectory();

    if (installDir.isEmpty() || !QDir(installDir).exists()) {
        PromptDialog::showDialog(
            parent,
            PromptDialog::Warning,
            titleResetAppInstallation(),
            msgInvalidDir(),
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok
            );
        return false;
    }

    const auto confirm = PromptDialog::showDialog(
        parent,
        PromptDialog::Question,
        titleConfirmAppReset(),
        msgConfirmReset(installDir),
        QString(),
        PromptDialog::Yes | PromptDialog::No,
        PromptDialog::No
        );

    return (confirm == PromptDialog::Yes);
}


// Performs the uninstall and notifies the user
bool UninstallServiceManager::performUninstall() const {
    const QString installDir = getInstallDirectory();

    if (deleteDirectory(installDir)) {
        PromptDialog::showDialog(
            nullptr,
            PromptDialog::Information,
            titleAppReset(),
            msgSuccess(),
            QString(),
            PromptDialog::Ok,
            PromptDialog::Ok
            );
        return true;
    }

    PromptDialog::showDialog(
        nullptr,
        PromptDialog::Critical,
        titleResetFailed(),
        msgFailure(),
        QString(),
        PromptDialog::Ok,
        PromptDialog::Ok
        );
    return false;
}


// Deletes the specified directory recursively
bool UninstallServiceManager::deleteDirectory(const QString& path) const {
    return QDir(path).removeRecursively();
}
