#ifndef TEMPLATEDIALOG_H
#define TEMPLATEDIALOG_H

// Project includes
#include "../../services/ServiceManagers/TemplateServiceManager/TemplateServiceManager.h"

// Qt includes
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVector>

// Forward declaration (Custom class)
struct TemplateEntry;
class TemplateServiceManager;

// Forward declaration (Qt class)
class QToolButton;

// Template dialog window
class TemplateDialog : public QDialog {
    Q_OBJECT

public:
    explicit TemplateDialog(TemplateServiceManager* service, QWidget* parent = nullptr);

signals:
    void templateLoaded(const QStringList& paths, const QString& name);
    void templateUnloaded(const QString& name);
    void requestSaveStaging();
    void requestRestoreStaging();

private slots:
    void onActionLoadOrUnloadClicked(const QString& name);
    void onActionDeleteClicked(int row);
    void onActionSetDefaultClicked(int row);
    void onTemplateSelectionChanged();
    void onNewTemplateClicked();

private:
    void populateTemplateTable();
    void updatePreviewForTemplate(const QString& templateName);

    TemplateServiceManager* templateService = nullptr;

    QTableWidget* tableWidget   = nullptr;
    QListWidget*  previewList   = nullptr;
    QPushButton*  newTemplateBtn = nullptr;
    QPushButton*  closeBtn       = nullptr;

    QString defaultTemplate;
    QString loadedTemplate;
};

#endif // TEMPLATEDIALOG_H
