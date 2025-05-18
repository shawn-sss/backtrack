#ifndef UNINSTALLSERVICECONSTANTS_H
#define UNINSTALLSERVICECONSTANTS_H

// Qt includes
#include <QString>
#include <QObject>

// Text used in uninstall confirmation dialogs
namespace UninstallServiceConstants {

// Dialog titles
inline QString titleResetAppInstallation() { return QObject::tr("Reset App Installation"); }
inline QString titleConfirmAppReset()      { return QObject::tr("Confirm App Reset"); }
inline QString titleAppReset()             { return QObject::tr("App Reset"); }
inline QString titleResetFailed()          { return QObject::tr("Reset Failed"); }

// Dialog messages
inline QString msgInvalidDir() {
    return QObject::tr("The application install directory is invalid or does not exist.");
}

inline QString msgConfirmReset(const QString& path) {
    return QObject::tr(
               "⚠️ This will completely reset the app by deleting the following directory:\n\n"
               "%1\n\n"
               "This will erase all user settings and metadata stored locally.\n"
               "This only targets the folder containing this application's runtime data.\n\n"
               "Are you sure you want to proceed?"
               ).arg(path);
}

inline QString msgSuccess() {
    return QObject::tr("The application data has been successfully deleted. Please restart the app.");
}

inline QString msgFailure() {
    return QObject::tr("Failed to delete the application install directory. Please check permissions.");
}

} // namespace UninstallServiceConstants

#endif // UNINSTALLSERVICECONSTANTS_H
