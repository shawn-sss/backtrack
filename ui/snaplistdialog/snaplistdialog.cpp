// Project includes
#include "snaplistdialog.h"
#include "snaplistdialogconstants.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h" // ✅ added
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceConstants.h"

// Qt includes
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>

SnapListDialog::SnapListDialog(SnapListServiceManager* service, QWidget* parent)
    : QDialog(parent), snapService(service) {
    using namespace SnapListDialogConstants;

    setWindowTitle(tr(k_WINDOW_TITLE));
    setMinimumSize(k_MIN_WIDTH, k_MIN_HEIGHT);

    listWidget = new QListWidget(this);
    loadButton = new QPushButton(tr(k_LOAD_BUTTON_TEXT), this);
    deleteButton = new QPushButton(tr(k_DELETE_BUTTON_TEXT), this);
    saveButton = new QPushButton(tr(k_SAVE_BUTTON_TEXT), this);

    // ✅ Centralized button styling and tooltip/cursor logic
    Shared::UI::applyButtonTooltipAndCursor(loadButton, tr(k_LOAD_BUTTON_TOOLTIP));
    Shared::UI::applyButtonTooltipAndCursor(deleteButton, tr(k_DELETE_BUTTON_TOOLTIP));
    Shared::UI::applyButtonTooltipAndCursor(saveButton, tr(k_SAVE_BUTTON_TOOLTIP));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addWidget(listWidget);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(loadButton, &QPushButton::clicked, this, &SnapListDialog::onLoadClicked);
    connect(deleteButton, &QPushButton::clicked, this, &SnapListDialog::onDeleteClicked);
    connect(saveButton, &QPushButton::clicked, this, &SnapListDialog::onSaveClicked);

    populateSnapList();
}

// Refreshes the list widget with available snap list names
void SnapListDialog::populateSnapList() {
    listWidget->clear();
    const QStringList snapLists = snapService->listSnapLists();
    for (const QString& name : snapLists) {
        listWidget->addItem(name);
    }
}

// Loads the selected snap list and emits the loaded data
void SnapListDialog::onLoadClicked() {
    QListWidgetItem* item = listWidget->currentItem();
    if (!item) return;

    const QString selectedListName = item->text();
    const QVector<SnapListEntry> entries = snapService->loadSnapList(selectedListName);

    QStringList loadedPaths;
    for (const SnapListEntry& entry : entries) {
        loadedPaths.append(entry.path);
    }

    emit snapListLoaded(loadedPaths, selectedListName);
    accept();
}

// Deletes the selected snap list after user confirmation
void SnapListDialog::onDeleteClicked() {
    using namespace SnapListDialogConstants;

    QListWidgetItem* item = listWidget->currentItem();
    if (!item) return;

    if (QMessageBox::question(
            this,
            tr(k_DELETE_CONFIRM_TITLE),
            tr(k_DELETE_CONFIRM_MESSAGE).arg(item->text()),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        snapService->deleteSnapList(item->text());
        populateSnapList();
    }
}

// Saves the current staging entries to a new snap list and optionally loads it
void SnapListDialog::onSaveClicked() {
    using namespace SnapListDialogConstants;

    QString name = QInputDialog::getText(
        this, tr(k_SAVE_PROMPT_TITLE), tr(k_SAVE_PROMPT_MESSAGE));

    if (name.isEmpty()) return;

    const QVector<SnapListEntry> entries = snapService->getCurrentStagingEntries();

    if (!snapService->saveSnapList(name, entries)) {
        QMessageBox::critical(this, tr(k_SAVE_FAILED_TITLE), tr(k_SAVE_FAILED_MESSAGE));
    } else {
        populateSnapList();

        const int choice = QMessageBox::question(
            this,
            tr(k_LOAD_NEW_PROMPT_TITLE),
            tr(k_LOAD_NEW_PROMPT_MESSAGE).arg(name),
            QMessageBox::Yes | QMessageBox::No);

        if (choice == QMessageBox::Yes) {
            QStringList loadedPaths;
            for (const SnapListEntry& entry : entries) {
                loadedPaths.append(entry.path);
            }

            emit snapListLoaded(loadedPaths, name);
            accept();
        }
    }
}
