#ifndef UNINSTALLSERVICEMANAGER_H
#define UNINSTALLSERVICEMANAGER_H

// Qt includes
#include <QString>
#include <QWidget>

// Manages app uninstall operations
class UninstallServiceManager {
public:
    UninstallServiceManager();

    // Query
    QString getInstallDirectory() const;

    // Uninstall
    [[nodiscard]] bool confirmUninstall(QWidget* parent) const;
    [[nodiscard]] bool performUninstall() const;

private:
    [[nodiscard]] bool deleteDirectory(const QString& path) const;
};

#endif
