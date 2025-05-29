// Project includes
#include "promptdialog.h"
#include "PromptDialogConstants.h"
#include "PromptDialogStyling.h"

// Qt includes
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QApplication>
#include <QScreen>

using namespace PromptDialogStyling::Styles;
using namespace PromptDialogConstants;

PromptDialog::PromptDialog(QWidget *parent)
    : QDialog(parent)
{
    initializeUI();
}

PromptDialog::~PromptDialog() {}

// Initializes and configures the dialog's UI
void PromptDialog::initializeUI()
{
    createWidgets();
    setupLayouts();
    applyStyling();
    configureConnections();

    setModal(true);
    setFixedSize(k_DIALOG_MIN_WIDTH, k_DIALOG_MIN_HEIGHT);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowMaximizeButtonHint);
}

// Creates the UI widgets used in the dialog
void PromptDialog::createWidgets()
{
    iconDisplay = new QLabel;
    iconDisplay->setAlignment(Qt::AlignCenter);
    iconDisplay->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    messageLabel = new QLabel;
    messageLabel->setWordWrap(true);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    messageLabel->setContentsMargins(0, 0, 0, -k_TEXT_SPACING);

    detailLabel = new QLabel;
    detailLabel->setWordWrap(true);
    detailLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    detailLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    buttonBox = new QDialogButtonBox;
    mainLayout = new QVBoxLayout;
}

// Sets up and arranges all layouts within the dialog
void PromptDialog::setupLayouts()
{
    auto *iconLayout = new QVBoxLayout;
    iconLayout->setContentsMargins(0, 0, 0, 0);
    iconLayout->setSpacing(0);
    iconLayout->addWidget(iconDisplay);
    iconLayout->addStretch();
    iconLayout->setAlignment(iconDisplay, Qt::AlignTop | Qt::AlignHCenter);

    auto *iconWrapper = new QWidget;
    iconWrapper->setLayout(iconLayout);

    auto *textLayout = new QVBoxLayout;
    textLayout->addWidget(messageLabel);
    textLayout->addWidget(detailLabel);
    textLayout->setSpacing(0);
    textLayout->setContentsMargins(0, 0, 0, 0);

    auto *textContainer = new QWidget;
    textContainer->setLayout(textLayout);

    auto *topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(k_OUTER_MARGIN / 2, 0, k_OUTER_MARGIN / 2, 0);
    topLayout->setSpacing(8);
    topLayout->addWidget(iconWrapper, 0);
    topLayout->addWidget(textContainer, 1);

    mainLayout->addLayout(topLayout);
    mainLayout->addSpacing(k_SECTION_SPACING);
    mainLayout->addWidget(buttonBox);
    mainLayout->setContentsMargins(k_OUTER_MARGIN, k_OUTER_MARGIN, k_OUTER_MARGIN, k_OUTER_MARGIN);
    mainLayout->setSpacing(k_SECTION_SPACING);

    setLayout(mainLayout);
}

// Applies stylesheet settings to the dialog and widgets
void PromptDialog::applyStyling()
{
    setStyleSheet(DIALOG_STYLE);
    iconDisplay->setStyleSheet(ICON_LABEL_STYLE);
    messageLabel->setStyleSheet(TEXT_LABEL_STYLE);
    detailLabel->setStyleSheet(INFO_TEXT_LABEL_STYLE);
}

// Sets up signal-slot connections
void PromptDialog::configureConnections()
{
    connect(buttonBox, &QDialogButtonBox::clicked, this, &PromptDialog::handleButtonClicked);
}

// Sets the main message text
void PromptDialog::setMessageText(const QString &text)
{
    messageLabel->setText(text);
}

// Sets the informative text (optional details)
void PromptDialog::setInformativeText(const QString &text)
{
    detailLabel->setText(text);
    detailLabel->setVisible(!text.isEmpty());
}

// Sets the icon displayed in the dialog
void PromptDialog::setIcon(Icon icon)
{
    QPixmap original = iconPixmap(icon);
    QPixmap scaled = original.scaled(k_ICON_SIZE, k_ICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconDisplay->setPixmap(scaled);
}

// Sets the standard buttons for the dialog
void PromptDialog::setStandardButtons(QMessageBox::StandardButtons buttons)
{
    buttonBox->setStandardButtons(static_cast<QDialogButtonBox::StandardButtons>(static_cast<int>(buttons)));
}

// Sets the default button for the dialog
void PromptDialog::setDefaultButton(QMessageBox::StandardButton button)
{
    auto *defaultBtn = buttonBox->button(static_cast<QDialogButtonBox::StandardButton>(static_cast<int>(button)));
    if (defaultBtn)
        defaultBtn->setDefault(true);
}

// Handles user button selection and stores the result
void PromptDialog::handleButtonClicked(QAbstractButton *button)
{
    userChoice = static_cast<QMessageBox::StandardButton>(
        static_cast<int>(buttonBox->standardButton(button)));
    accept();
}

// Returns a QPixmap based on the specified icon type
QPixmap PromptDialog::iconPixmap(Icon iconType)
{
    QStyle *style = QApplication::style();
    switch (iconType) {
    case Information:
        return style->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(k_ICON_SIZE, k_ICON_SIZE);
    case Warning:
        return style->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(k_ICON_SIZE, k_ICON_SIZE);
    case Critical:
        return style->standardIcon(QStyle::SP_MessageBoxCritical).pixmap(k_ICON_SIZE, k_ICON_SIZE);
    case Question:
        return style->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(k_ICON_SIZE, k_ICON_SIZE);
    default:
        return QPixmap();
    }
}

// Static method to show the dialog and return the user's selection
QMessageBox::StandardButton PromptDialog::showDialog(QWidget *parent,
                                                     Icon icon,
                                                     const QString &title,
                                                     const QString &messageText,
                                                     const QString &informativeText,
                                                     QMessageBox::StandardButtons buttons,
                                                     QMessageBox::StandardButton defaultButton)
{
    PromptDialog dialog(parent);
    dialog.setWindowTitle(title);
    dialog.setIcon(icon);
    dialog.setMessageText(messageText);
    dialog.setInformativeText(informativeText);
    dialog.setStandardButtons(buttons);
    dialog.setDefaultButton(defaultButton == QMessageBox::NoButton ? QMessageBox::Ok : defaultButton);

    const QRect targetRect = parent ? parent->geometry() : QApplication::primaryScreen()->geometry();
    const QPoint centerPoint = targetRect.center() - QPoint(dialog.width() / 2, dialog.height() / 2);
    dialog.move(centerPoint);

    dialog.exec();
    return dialog.userChoice;
}
