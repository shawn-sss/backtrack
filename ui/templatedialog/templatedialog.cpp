// filename: templatedialog.cpp

// Project includes
#include "templatedialog.h"
#include "templatedialogconstants.h"
#include "../../services/ServiceManagers/TemplateServiceManager/TemplateServiceManager.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"

// Qt includes
#include <QAbstractItemView>
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QStringList>

// Constructs TemplateDialog and sets up UI
TemplateDialog::TemplateDialog(TemplateServiceManager* service, QWidget* parent)
    : QDialog(parent), templateService(service) {
    using namespace TemplateDialogConstants;

    setWindowTitle(tr(k_WINDOW_TITLE));
    setMinimumSize(k_MIN_WIDTH, k_MIN_HEIGHT);

    listWidget   = new QListWidget(this);
    loadButton   = new QPushButton(tr(k_LOAD_BUTTON_TEXT), this);
    deleteButton = new QPushButton(tr(k_DELETE_BUTTON_TEXT), this);
    saveButton   = new QPushButton(tr(k_SAVE_BUTTON_TEXT), this);

    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    listWidget->setUniformItemSizes(true);

    Shared::UI::applyButtonTooltipAndCursor(loadButton,   tr(k_LOAD_BUTTON_TOOLTIP));
    Shared::UI::applyButtonTooltipAndCursor(deleteButton, tr(k_DELETE_BUTTON_TOOLTIP));
    Shared::UI::applyButtonTooltipAndCursor(saveButton,   tr(k_SAVE_BUTTON_TOOLTIP));

    auto* mainLayout   = new QVBoxLayout(this);
    auto* buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addWidget(listWidget);
    mainLayout->addLayout(buttonLayout);

    const bool serviceOk = (templateService != nullptr);
    loadButton->setEnabled(serviceOk);
    deleteButton->setEnabled(serviceOk);
    saveButton->setEnabled(serviceOk);

    connect(loadButton,   &QPushButton::clicked, this, &TemplateDialog::onLoadClicked);
    connect(deleteButton, &QPushButton::clicked, this, &TemplateDialog::onDeleteClicked);
    connect(saveButton,   &QPushButton::clicked, this, &TemplateDialog::onSaveClicked);

    connect(listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem*) { onLoadClicked(); });

    populateTemplateList();
}

// Refreshes the list of templates
void TemplateDialog::populateTemplateList() {
    listWidget->clear();
    if (!templateService) return;

    const QStringList templates = templateService->listTemplates();
    listWidget->addItems(templates);

    if (!templates.isEmpty()) {
        listWidget->setCurrentRow(0);
    }
}

// Loads the selected template
void TemplateDialog::onLoadClicked() {
    if (!templateService) return;

    QListWidgetItem* item = listWidget->currentItem();
    if (!item) return;

    const QString selectedTemplateName = item->text();
    const QVector<TemplateEntry> entries = templateService->loadTemplate(selectedTemplateName);

    QStringList loadedPaths;
    loadedPaths.reserve(entries.size());
    for (const TemplateEntry& entry : entries) {
        loadedPaths.append(entry.path);
    }

    emit templateLoaded(loadedPaths, selectedTemplateName);
    accept();
}

// Deletes the selected template
void TemplateDialog::onDeleteClicked() {
    using namespace TemplateDialogConstants;
    if (!templateService) return;

    QListWidgetItem* item = listWidget->currentItem();
    if (!item) return;

    const QString name = item->text();
    if (QMessageBox::question(
            this,
            tr(k_DELETE_CONFIRM_TITLE),
            k_DELETE_CONFIRM_MESSAGE.arg(name),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        templateService->deleteTemplate(name);
        populateTemplateList();
    }
}

// Saves the current staging entries to a new template
void TemplateDialog::onSaveClicked() {
    using namespace TemplateDialogConstants;
    if (!templateService) return;

    QString name = QInputDialog::getText(
                       this, tr(k_SAVE_PROMPT_TITLE), tr(k_SAVE_PROMPT_MESSAGE)).trimmed();

    if (name.isEmpty()) return;

    const QVector<TemplateEntry> entries = templateService->getCurrentStagingEntries();

    if (!templateService->saveTemplate(name, entries)) {
        QMessageBox::critical(this, tr(k_SAVE_FAILED_TITLE), tr(k_SAVE_FAILED_MESSAGE));
        return;
    }

    populateTemplateList();

    const int choice = QMessageBox::question(
        this,
        tr(k_LOAD_NEW_PROMPT_TITLE),
        k_LOAD_NEW_PROMPT_MESSAGE.arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if (choice == QMessageBox::Yes) {
        QStringList loadedPaths;
        loadedPaths.reserve(entries.size());
        for (const TemplateEntry& entry : entries) {
            loadedPaths.append(entry.path);
        }
        emit templateLoaded(loadedPaths, name);
        accept();
    }
}
