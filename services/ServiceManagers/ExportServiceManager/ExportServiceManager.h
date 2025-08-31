#ifndef EXPORTSERVICEMANAGER_H
#define EXPORTSERVICEMANAGER_H

// Qt includes
#include <QObject>
#include <QString>
#include <QJsonObject>

class QWidget;

class ExportServiceManager : public QObject {
    Q_OBJECT

public:
    explicit ExportServiceManager(QObject* parent = nullptr);
    bool exportUserPreferences(QWidget* parent);

signals:
    void exportSucceeded(const QString& filePath);
    void exportFailed(const QString& reason);

private:
    // File handling helpers
    QString getSaveFilePath(QWidget* parent) const;
    QJsonObject createExportObject() const;

    // Dialog helpers
    void showInfoDialog(QWidget* parent, const QString& title, const QString& message) const;
    void showWarningDialog(QWidget* parent, const QString& title, const QString& message) const;
};

#endif
