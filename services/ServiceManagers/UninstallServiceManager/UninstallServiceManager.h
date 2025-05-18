#ifndef UNINSTALLSERVICEMANAGER_H
#define UNINSTALLSERVICEMANAGER_H

// Qt includes
#include <QString>
#include <QWidget>

// Handles app data uninstallation
class UninstallServiceManager {
public:
    UninstallServiceManager();

    // Prompts the user and deletes the install directory if confirmed
    bool promptAndUninstall(QWidget* parent) const;

private:
    QString getInstallDirectory() const;
    bool deleteDirectory(const QString& path) const;
};

#endif // UNINSTALLSERVICEMANAGER_H
