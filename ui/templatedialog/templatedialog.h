// filename: templatedialog.h

#ifndef TEMPLATEDIALOG_H
#define TEMPLATEDIALOG_H

// Qt includes
#include <QDialog>
#include <QString>
#include <QStringList>

// Forward declaration (Custom class)
class TemplateServiceManager;

// Forward declarations (Qt classes)
class QListWidget;
class QPushButton;

class TemplateDialog : public QDialog {
    Q_OBJECT

public:
    explicit TemplateDialog(TemplateServiceManager* service, QWidget* parent = nullptr);

signals:
    void templateLoaded(const QStringList& paths, const QString& name);

private slots:
    void onLoadClicked();
    void onDeleteClicked();
    void onSaveClicked();

private:
    void populateTemplateList();

    TemplateServiceManager* templateService = nullptr;

    QListWidget* listWidget   = nullptr;
    QPushButton* loadButton   = nullptr;
    QPushButton* deleteButton = nullptr;
    QPushButton* saveButton   = nullptr;

    Q_DISABLE_COPY_MOVE(TemplateDialog)
};

#endif // TEMPLATEDIALOG_H
