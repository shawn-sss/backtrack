#ifndef UNINSTALLSERVICECONSTANTS_H
#define UNINSTALLSERVICECONSTANTS_H

#include <QString>
#include <QObject>

namespace UninstallServiceConstants {
// Dialog Titles
const QString TITLE_RESET_APP_INSTALLATION = QObject::tr("Reset App Installation");
const QString TITLE_CONFIRM_APP_RESET = QObject::tr("Confirm App Reset");
const QString TITLE_APP_RESET = QObject::tr("App Reset");
const QString TITLE_RESET_FAILED = QObject::tr("Reset Failed");

// Dialog Messages
const QString MSG_INVALID_DIR = QObject::tr("The application install directory is invalid or does not exist.");
const QString MSG_CONFIRM_RESET = QObject::tr(
    "⚠️ This will completely reset the app by deleting the following directory:\n\n"
    "%1\n\n"
    "This will erase all user settings and metadata stored locally.\n"
    "This only targets the folder containing this applications runtime data.\n\n"
    "Are you sure you want to proceed?");
const QString MSG_SUCCESS = QObject::tr("The application data has been successfully deleted. Please restart the app.");
const QString MSG_FAILURE = QObject::tr("Failed to delete the application install directory. Please check permissions.");
}

#endif // UNINSTALLSERVICECONSTANTS_H
