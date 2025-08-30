// Project includes
#include "UninstallServiceManager.h"
#include "UninstallServiceConstants.h"
#include "../../../ui/promptdialog/promptdialog.h"

// Qt includes
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

using namespace UninstallServiceConstants;

namespace {
bool isSafeAppDataDir(const QString& path) {
    const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (path.isEmpty() || appData.isEmpty())
        return false;

    const QString canonPath   = QFileInfo(path).canonicalFilePath();
    const QString canonAppDir = QFileInfo(appData).canonicalFilePath();
    return !canonPath.isEmpty() && !canonAppDir.isEmpty() && canonPath == canonAppDir;
}
}

// Lifecycle
UninstallServiceManager::UninstallServiceManager() = default;

// Query: installation directory
QString UninstallServiceManager::getInstallDirectory() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Uninstall: confirm with user
bool UninstallServiceManager::confirmUninstall(QWidget* parent) const {
    const QString installDir = getInstallDirectory();

    if (installDir.isEmpty() || !QDir(installDir).exists()) {
        PromptDialog::showDialog(parent,
                                 PromptDialog::Warning,
                                 titleResetAppInstallation(),
                                 msgInvalidDir(),
                                 QString(),
                                 PromptDialog::Ok,
                                 PromptDialog::Ok);
        return false;
    }

    const auto confirm = PromptDialog::showDialog(parent,
                                                  PromptDialog::Question,
                                                  titleConfirmAppReset(),
                                                  msgConfirmReset(installDir),
                                                  QString(),
                                                  PromptDialog::Yes | PromptDialog::No,
                                                  PromptDialog::No);

    return (confirm == PromptDialog::Yes);
}

// Uninstall: perform deletion
bool UninstallServiceManager::performUninstall() const {
    const QString installDir = getInstallDirectory();

    if (installDir.isEmpty() || !QDir(installDir).exists() || !isSafeAppDataDir(installDir)) {
        PromptDialog::showDialog(nullptr,
                                 PromptDialog::Warning,
                                 titleResetAppInstallation(),
                                 msgInvalidDir(),
                                 QString(),
                                 PromptDialog::Ok,
                                 PromptDialog::Ok);
        return false;
    }

    if (deleteDirectory(installDir)) {
        PromptDialog::showDialog(nullptr,
                                 PromptDialog::Information,
                                 titleAppReset(),
                                 msgSuccess(),
                                 QString(),
                                 PromptDialog::Ok,
                                 PromptDialog::Ok);
        return true;
    }

    PromptDialog::showDialog(nullptr,
                             PromptDialog::Critical,
                             titleResetFailed(),
                             msgFailure(),
                             QString(),
                             PromptDialog::Ok,
                             PromptDialog::Ok);
    return false;
}

// Internal: recursive deletion
bool UninstallServiceManager::deleteDirectory(const QString& path) const {
    if (path.isEmpty() || !QDir(path).exists())
        return false;
    return QDir(path).removeRecursively();
}
