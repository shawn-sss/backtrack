// Project includes
#include "promptdialog.h"
#include "PromptDialogConstants.h"
#include "PromptDialogStyling.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h" // ✅ added

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

// Constructor and destructor
PromptDialog::PromptDialog(QWidget *parent)
    : QDialog(parent) {
    initializeUI();
}

PromptDialog::~PromptDialog() {}

// Initializes and configures dialog UI
void PromptDialog::initializeUI() {
    createWidgets();
    setupLayouts();
    applyStyling();
    configureConnections();

    setModal(true);
    setFixedSize(k_DIALOG_MIN_WIDTH, k_DIALOG_MIN_HEIGHT);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowMaximizeButtonHint);
}

// Creates core widgets
void PromptDialog::createWidgets() {
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

// Builds and arranges layouts
void PromptDialog::setupLayouts() {
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

// Applies stylesheet to dialog components
void PromptDialog::applyStyling() {
    setStyleSheet(DIALOG_STYLE);
    iconDisplay->setStyleSheet(ICON_LABEL_STYLE);
    messageLabel->setStyleSheet(TEXT_LABEL_STYLE);
    detailLabel->setStyleSheet(INFO_TEXT_LABEL_STYLE);
}

// Connects dialog signals and slots
void PromptDialog::configureConnections() {
    connect(buttonBox, &QDialogButtonBox::clicked, this, &PromptDialog::handleButtonClicked);
}

// Sets main message text
void PromptDialog::setMessageText(const QString &text) {
    messageLabel->setText(text);
}

// Sets additional informative text
void PromptDialog::setInformativeText(const QString &text) {
    detailLabel->setText(text);
    detailLabel->setVisible(!text.isEmpty());
}

// Sets icon based on message type
void PromptDialog::setIcon(Icon icon) {
    QPixmap original = iconPixmap(icon);
    QPixmap scaled = original.scaled(k_ICON_SIZE, k_ICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconDisplay->setPixmap(scaled);
}

// Configures standard buttons on dialog
void PromptDialog::setStandardButtons(Buttons buttons) {
    buttonBox->setStandardButtons(toStandardButtons(buttons));

    // ✅ Apply shared button styling after button creation
    const auto btns = buttonBox->buttons();
    for (QAbstractButton* button : btns) {
        if (auto* pushBtn = qobject_cast<QPushButton*>(button)) {
            QString text = pushBtn->text();
            Shared::UI::applyButtonTooltipAndCursor(pushBtn, text);  // Use button text as tooltip
        }
    }
}

// Sets default button to focus
void PromptDialog::setDefaultButton(Button button) {
    auto *defaultBtn = buttonBox->button(toStandardButton(button));
    if (defaultBtn)
        defaultBtn->setDefault(true);
}

// Handles button click event
void PromptDialog::handleButtonClicked(QAbstractButton *button) {
    switch (buttonBox->standardButton(button)) {
    case QDialogButtonBox::Ok: userChoice = Ok; break;
    case QDialogButtonBox::Cancel: userChoice = Cancel; break;
    case QDialogButtonBox::Yes: userChoice = Yes; break;
    case QDialogButtonBox::No: userChoice = No; break;
    default: userChoice = None; break;
    }
    accept();
}

// Returns icon pixmap for given type
QPixmap PromptDialog::iconPixmap(Icon iconType) {
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

// Displays and centers dialog, returns selected button
PromptDialog::Button PromptDialog::showDialog(QWidget *parent,
                                              Icon icon,
                                              const QString &title,
                                              const QString &messageText,
                                              const QString &informativeText,
                                              Buttons buttons,
                                              Button defaultButton) {
    PromptDialog dialog(parent);
    dialog.setWindowTitle(title);
    dialog.setIcon(icon);
    dialog.setMessageText(messageText);
    dialog.setInformativeText(informativeText);
    dialog.setStandardButtons(buttons);

    if (defaultButton == None) {
        if (buttons.testFlag(Ok)) defaultButton = Ok;
        else if (buttons.testFlag(Yes)) defaultButton = Yes;
        else if (buttons.testFlag(No)) defaultButton = No;
        else if (buttons.testFlag(Cancel)) defaultButton = Cancel;
    }

    dialog.setDefaultButton(defaultButton);

    const QRect targetRect = parent ? parent->geometry() : QApplication::primaryScreen()->geometry();
    const QPoint centerPoint = targetRect.center() - QPoint(dialog.width() / 2, dialog.height() / 2);
    dialog.move(centerPoint);

    dialog.exec();
    return dialog.userChoice;
}

// Converts Buttons flags to QDialogButtonBox::StandardButtons
QDialogButtonBox::StandardButtons PromptDialog::toStandardButtons(Buttons buttons) {
    QDialogButtonBox::StandardButtons stdButtons;
    if (buttons.testFlag(Ok)) stdButtons |= QDialogButtonBox::Ok;
    if (buttons.testFlag(Cancel)) stdButtons |= QDialogButtonBox::Cancel;
    if (buttons.testFlag(Yes)) stdButtons |= QDialogButtonBox::Yes;
    if (buttons.testFlag(No)) stdButtons |= QDialogButtonBox::No;
    return stdButtons;
}

// Converts single Button to QDialogButtonBox::StandardButton
QDialogButtonBox::StandardButton PromptDialog::toStandardButton(Button button) {
    switch (button) {
    case Ok: return QDialogButtonBox::Ok;
    case Cancel: return QDialogButtonBox::Cancel;
    case Yes: return QDialogButtonBox::Yes;
    case No: return QDialogButtonBox::No;
    default: return QDialogButtonBox::NoButton;
    }
}
