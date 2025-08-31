// Project includes
#include "templatedialog.h"
#include "templatedialogconstants.h"
#include "templatedialogstyling.h"
#include "../../services/ServiceManagers/TemplateServiceManager/TemplateServiceManager.h"

// Qt includes
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QToolButton>
#include <QMessageBox>
#include <QDateTime>
#include <QLabel>
#include <QFont>
#include <QInputDialog>

// Constructor
TemplateDialog::TemplateDialog(TemplateServiceManager* service, QWidget* parent)
    : QDialog(parent), templateService(service) {
    using namespace TemplateDialogConstants;
    using namespace TemplateDialogStyling::Styles;

    setWindowTitle(tr(k_WINDOW_TITLE));
    setMinimumSize(k_MIN_WIDTH, k_MIN_HEIGHT);

    auto* mainLayout = new QVBoxLayout(this);

    auto* contentLayout = new QHBoxLayout();

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({
        tr(k_HEADER_NAME),
        tr(k_HEADER_ITEMS),
        tr(k_HEADER_MODIFIED),
        tr(k_HEADER_ACTIONS)
    });
    tableWidget->horizontalHeader()->setStretchLastSection(false);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setSortingEnabled(true);
    tableWidget->setStyleSheet(k_TABLE_STYLE);
    tableWidget->sortItems(2, Qt::DescendingOrder);

    contentLayout->addWidget(tableWidget, 2);

    auto* rightPanel = new QVBoxLayout();
    rightPanel->addWidget(new QLabel(tr(k_PREVIEW_LABEL), this));
    previewList = new QListWidget(this);
    rightPanel->addWidget(previewList, 1);
    contentLayout->addLayout(rightPanel, 1);

    mainLayout->addLayout(contentLayout);

    auto* footerLayout = new QHBoxLayout();
    newTemplateBtn = new QPushButton(tr(k_NEW_TEMPLATE_BUTTON_TEXT), this);
    closeBtn       = new QPushButton(tr(k_CLOSE_BUTTON_TEXT), this);
    footerLayout->addWidget(newTemplateBtn);
    footerLayout->addStretch();
    footerLayout->addWidget(closeBtn);
    mainLayout->addLayout(footerLayout);

    connect(closeBtn, &QPushButton::clicked, this, &TemplateDialog::reject);
    connect(newTemplateBtn, &QPushButton::clicked, this, &TemplateDialog::onNewTemplateClicked);
    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, &TemplateDialog::onTemplateSelectionChanged);

    populateTemplateTable();
}

// Populate the template table
void TemplateDialog::populateTemplateTable() {
    using namespace TemplateDialogConstants;
    using namespace TemplateDialogStyling::Styles;

    tableWidget->setRowCount(0);
    previewList->clear();

    if (!templateService) return;

    QStringList templates = templateService->listTemplates();
    defaultTemplate = templateService->getDefaultTemplate();

    tableWidget->setSortingEnabled(false);
    int defaultRow = -1;

    for (int i = 0; i < templates.size(); ++i) {
        const QString& rawName = templates.at(i);
        QVector<TemplateEntry> entries = templateService->loadTemplate(rawName);

        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        QString displayName = rawName;
        auto* nameItem = new QTableWidgetItem(displayName);
        nameItem->setData(Qt::UserRole, rawName);
        if (rawName == defaultTemplate) {
            nameItem->setText(displayName + k_DEFAULT_SUFFIX);
            nameItem->setFont(QFont(nameItem->font().family(), nameItem->font().pointSize(), QFont::Bold));
            defaultRow = row;
        }
        nameItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 0, nameItem);

        auto* itemsItem = new QTableWidgetItem(QString::number(entries.size()));
        itemsItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 1, itemsItem);

        QDateTime lastModified;
        for (const auto& e : entries) {
            if (e.lastModified.isValid() && (!lastModified.isValid() || e.lastModified > lastModified)) {
                lastModified = e.lastModified;
            }
        }
        if (!lastModified.isValid()) lastModified = QDateTime::currentDateTimeUtc();

        auto* dateItem = new QTableWidgetItem(lastModified.toString(k_DATE_FORMAT));
        dateItem->setData(Qt::UserRole, lastModified);
        dateItem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 2, dateItem);

        QWidget* actionWidget = new QWidget(this);
        auto* actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(k_ACTION_LAYOUT_MARGIN, k_ACTION_LAYOUT_MARGIN,
                                         k_ACTION_LAYOUT_MARGIN, k_ACTION_LAYOUT_MARGIN);
        actionLayout->setSpacing(k_ACTION_LAYOUT_SPACING);
        actionLayout->setAlignment(Qt::AlignCenter);

        QSize btnSize(k_ACTION_BUTTON_SIZE, k_ACTION_BUTTON_SIZE);

        auto* loadBtn = new QToolButton(actionWidget);
        loadBtn->setFixedSize(btnSize);
        loadBtn->setCursor(Qt::PointingHandCursor);
        loadBtn->setProperty("templateName", rawName);

        if (loadedTemplate == rawName) {
            loadBtn->setText(k_ICON_UNLOAD);
            loadBtn->setToolTip(tr(k_TOOLTIP_UNLOAD));
        } else {
            loadBtn->setText(k_ICON_LOAD);
            loadBtn->setToolTip(tr(k_TOOLTIP_LOAD));
        }
        connect(loadBtn, &QToolButton::clicked, this, [this, loadBtn]() {
            QString name = loadBtn->property("templateName").toString();
            onActionLoadOrUnloadClicked(name);
        });
        actionLayout->addWidget(loadBtn);

        auto* deleteBtn = new QToolButton(actionWidget);
        deleteBtn->setFixedSize(btnSize);
        deleteBtn->setText(k_ICON_DELETE);
        deleteBtn->setCursor(Qt::PointingHandCursor);
        deleteBtn->setToolTip(tr(k_TOOLTIP_DELETE));
        connect(deleteBtn, &QToolButton::clicked, this, [this, row]() { onActionDeleteClicked(row); });
        actionLayout->addWidget(deleteBtn);

        auto* defaultBtn = new QToolButton(actionWidget);
        defaultBtn->setFixedSize(btnSize);
        defaultBtn->setText(k_ICON_DEFAULT);
        defaultBtn->setCursor(Qt::PointingHandCursor);
        defaultBtn->setToolTip(tr(k_TOOLTIP_DEFAULT));
        if (rawName == defaultTemplate) {
            defaultBtn->setStyleSheet(k_DEFAULT_BUTTON_GOLD);
        } else {
            defaultBtn->setStyleSheet(k_DEFAULT_BUTTON_GRAY);
        }
        connect(defaultBtn, &QToolButton::clicked, this, [this, row]() { onActionSetDefaultClicked(row); });
        actionLayout->addWidget(defaultBtn);

        tableWidget->setCellWidget(row, 3, actionWidget);
    }

    if (defaultRow >= 0) {
        tableWidget->insertRow(0);
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->takeItem(defaultRow + 1, col);
            if (item) tableWidget->setItem(0, col, item);
            QWidget* cellWidget = tableWidget->cellWidget(defaultRow + 1, col);
            if (cellWidget) tableWidget->setCellWidget(0, col, cellWidget);
        }
        tableWidget->removeRow(defaultRow + 1);
    }

    tableWidget->setSortingEnabled(true);
    tableWidget->sortItems(2, Qt::DescendingOrder);
}

// Handle load or unload action
void TemplateDialog::onActionLoadOrUnloadClicked(const QString& name) {
    using namespace TemplateDialogConstants;

    if (!templateService) return;

    if (loadedTemplate == name) {
        emit requestRestoreStaging();
        loadedTemplate.clear();
        emit templateUnloaded(name);
    } else {
        emit requestSaveStaging();

        QVector<TemplateEntry> entries = templateService->loadTemplate(name);
        if (entries.isEmpty()) {
            QMessageBox::warning(this, tr(k_SAVE_FAILED_TITLE), tr(k_EMPTY_TEMPLATE_MESSAGE));
            return;
        }

        QStringList paths;
        for (const auto& e : entries) {
            paths.append(e.path);
        }

        loadedTemplate = name;
        emit templateLoaded(paths, name);
    }

    populateTemplateTable();
}

// Handle delete action
void TemplateDialog::onActionDeleteClicked(int row) {
    using namespace TemplateDialogConstants;

    if (!templateService) return;
    if (row < 0 || row >= tableWidget->rowCount()) return;

    QString name = tableWidget->item(row, 0)->text();
    name.remove(k_DEFAULT_SUFFIX);

    if (QMessageBox::question(this,
                              tr(k_DELETE_CONFIRM_TITLE),
                              QString(k_DELETE_CONFIRM_MESSAGE).arg(name),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        templateService->deleteTemplate(name);

        if (loadedTemplate == name) {
            emit requestRestoreStaging();
            loadedTemplate.clear();
            emit templateUnloaded(name);
        }

        populateTemplateTable();
    }
}

// Handle set default action
void TemplateDialog::onActionSetDefaultClicked(int row) {
    using namespace TemplateDialogConstants;

    if (!templateService) return;
    if (row < 0 || row >= tableWidget->rowCount()) return;

    QString rawName = tableWidget->item(row, 0)->data(Qt::UserRole).toString();

    if (templateService->getDefaultTemplate() == rawName) {
        templateService->clearDefaultTemplate();
    } else {
        templateService->setDefaultTemplate(rawName);
    }

    populateTemplateTable();
}

// Handle selection change
void TemplateDialog::onTemplateSelectionChanged() {
    int row = tableWidget->currentRow();
    if (row < 0) {
        previewList->clear();
        return;
    }

    QString rawName = tableWidget->item(row, 0)->data(Qt::UserRole).toString();
    updatePreviewForTemplate(rawName);
}

// Update preview for a template
void TemplateDialog::updatePreviewForTemplate(const QString& templateName) {
    using namespace TemplateDialogConstants;

    previewList->clear();
    if (!templateService) return;

    QVector<TemplateEntry> entries = templateService->loadTemplate(templateName);
    for (const auto& entry : entries) {
        QString display = entry.path;
        if (entry.isFolder) display += k_FOLDER_SUFFIX;
        previewList->addItem(display);
    }
}

// Handle new template creation
void TemplateDialog::onNewTemplateClicked() {
    using namespace TemplateDialogConstants;

    if (!templateService) return;

    QString name = QInputDialog::getText(this, tr(k_NEW_TEMPLATE_DIALOG_TITLE),
                                         tr(k_RENAME_PROMPT_MESSAGE)).trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, tr(k_INVALID_NAME_TITLE), tr(k_INVALID_NAME_MESSAGE));
        return;
    }

    QString lowerName = name.toLower();
    QStringList existingTemplates = templateService->listTemplates();
    for (const QString& existing : existingTemplates) {
        if (existing.toLower() == lowerName) {
            QMessageBox::warning(this, tr(k_DUPLICATE_NAME_TITLE),
                                 QString(k_DUPLICATE_NAME_MESSAGE).arg(name));
            return;
        }
    }

    QVector<TemplateEntry> entries = templateService->getCurrentStagingEntries();
    if (entries.isEmpty()) {
        QMessageBox::warning(this, tr(k_SAVE_FAILED_TITLE), tr(k_EMPTY_TEMPLATE_MESSAGE));
        return;
    }

    if (!templateService->saveTemplate(name, entries)) {
        QMessageBox::critical(this, tr(k_SAVE_FAILED_TITLE), tr(k_SAVE_FAILED_MESSAGE));
        return;
    }

    populateTemplateTable();
}
