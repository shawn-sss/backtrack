#ifndef UNINSTALLSERVICEMANAGER_H
#define UNINSTALLSERVICEMANAGER_H

// Qt includes
#include <QString>
#include <QWidget>

// Manages uninstallation-related operations
class UninstallServiceManager {
public:
    UninstallServiceManager();

    // Prompts user for confirmation and performs uninstallation if accepted
    bool promptAndUninstall(QWidget* parent) const;

private:
    // Returns the application's install directory path
    QString getInstallDirectory() const;

    // Deletes the specified directory and its contents
    bool deleteDirectory(const QString& path) const;
};

#endif // UNINSTALLSERVICEMANAGER_H
