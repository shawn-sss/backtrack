#ifndef UNINSTALLSERVICEMANAGER_H
#define UNINSTALLSERVICEMANAGER_H

// Qt includes
#include <QString>
#include <QWidget>

// Handles app data uninstallation
class UninstallServiceManager {
public:
    UninstallServiceManager();

    // Directory management
    QString getInstallDirectory() const;

    // Uninstall operations
    bool confirmUninstall(QWidget* parent) const;
    bool performUninstall() const;

private:
    bool deleteDirectory(const QString& path) const;
};

#endif // UNINSTALLSERVICEMANAGER_H
