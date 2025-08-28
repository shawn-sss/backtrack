// filename: snaplistdialog.cpp

// Project includes
#include "snaplistdialog.h"
#include "snaplistdialogconstants.h"
#include "../../services/ServiceManagers/SnapListServiceManager/snaplistservicemanager.h"
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

// Constructs SnapListDialog and sets up UI
SnapListDialog::SnapListDialog(SnapListServiceManager* service, QWidget* parent)
    : QDialog(parent), snapService(service) {
    using namespace SnapListDialogConstants;

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

    const bool serviceOk = (snapService != nullptr);
    loadButton->setEnabled(serviceOk);
    deleteButton->setEnabled(serviceOk);
    saveButton->setEnabled(serviceOk);

    connect(loadButton,   &QPushButton::clicked, this, &SnapListDialog::onLoadClicked);
    connect(deleteButton, &QPushButton::clicked, this, &SnapListDialog::onDeleteClicked);
    connect(saveButton,   &QPushButton::clicked, this, &SnapListDialog::onSaveClicked);

    connect(listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem*) { onLoadClicked(); });

    populateSnapList();
}

// Refreshes the list of snap lists
void SnapListDialog::populateSnapList() {
    listWidget->clear();
    if (!snapService) return;

    const QStringList snapLists = snapService->listSnapLists();
    listWidget->addItems(snapLists);

    if (!snapLists.isEmpty()) {
        listWidget->setCurrentRow(0);
    }
}

// Loads the selected snap list
void SnapListDialog::onLoadClicked() {
    if (!snapService) return;

    QListWidgetItem* item = listWidget->currentItem();
    if (!item) return;

    const QString selectedListName = item->text();
    const QVector<SnapListEntry> entries = snapService->loadSnapList(selectedListName);

    QStringList loadedPaths;
    loadedPaths.reserve(entries.size());
    for (const SnapListEntry& entry : entries) {
        loadedPaths.append(entry.path);
    }

    emit snapListLoaded(loadedPaths, selectedListName);
    accept();
}

// Deletes the selected snap list
void SnapListDialog::onDeleteClicked() {
    using namespace SnapListDialogConstants;
    if (!snapService) return;

    QListWidgetItem* item = listWidget->currentItem();
    if (!item) return;

    const QString name = item->text();
    if (QMessageBox::question(
            this,
            tr(k_DELETE_CONFIRM_TITLE),
            k_DELETE_CONFIRM_MESSAGE.arg(name),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        snapService->deleteSnapList(name);
        populateSnapList();
    }
}

// Saves the current staging entries to a new snap list
void SnapListDialog::onSaveClicked() {
    using namespace SnapListDialogConstants;
    if (!snapService) return;

    QString name = QInputDialog::getText(
                       this, tr(k_SAVE_PROMPT_TITLE), tr(k_SAVE_PROMPT_MESSAGE)).trimmed();

    if (name.isEmpty()) return;

    const QVector<SnapListEntry> entries = snapService->getCurrentStagingEntries();

    if (!snapService->saveSnapList(name, entries)) {
        QMessageBox::critical(this, tr(k_SAVE_FAILED_TITLE), tr(k_SAVE_FAILED_MESSAGE));
        return;
    }

    populateSnapList();

    const int choice = QMessageBox::question(
        this,
        tr(k_LOAD_NEW_PROMPT_TITLE),
        k_LOAD_NEW_PROMPT_MESSAGE.arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if (choice == QMessageBox::Yes) {
        QStringList loadedPaths;
        loadedPaths.reserve(entries.size());
        for (const SnapListEntry& entry : entries) {
            loadedPaths.append(entry.path);
        }
        emit snapListLoaded(loadedPaths, name);
        accept();
    }
}
