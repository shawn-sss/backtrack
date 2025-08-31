#ifndef IMPORTSERVICEMANAGER_H
#define IMPORTSERVICEMANAGER_H

// Qt includes
#include <QObject>
#include <QString>

class QWidget;
class QJsonObject;

class ImportServiceManager : public QObject {
    Q_OBJECT

public:
    explicit ImportServiceManager(QObject* parent = nullptr);
    bool importUserPreferences(QWidget* parent = nullptr);

signals:
    void importSucceeded(const QString& filePath);
    void importFailed(const QString& reason);

private:
    // File handling helpers
    QString getOpenFilePath(QWidget* parent) const;
    bool validateRootObject(const QJsonObject& root, QWidget* parent);

    // Dialog helpers
    void showInfoDialog(QWidget* parent, const QString& title, const QString& message) const;
    void showWarningDialog(QWidget* parent, const QString& title, const QString& message) const;
};

#endif
