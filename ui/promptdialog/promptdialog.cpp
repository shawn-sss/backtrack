// Project includes
#include "PromptDialog.h"
#include "PromptDialogConstants.h"
#include "PromptDialogStyling.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"

// Qt
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QApplication>
#include <QScreen>
#include <QScrollArea>
#include <QAbstractButton>
#include <QFrame>
#include <QFontMetrics>

using namespace PromptDialogStyling::Styles;
using namespace PromptDialogConstants;

PromptDialog::PromptDialog(QWidget *parent) : QDialog(parent) {
    initializeUI();
}

PromptDialog::~PromptDialog() {}

void PromptDialog::initializeUI() {
    createWidgets();
    setupLayouts();
    applyStyling();
    configureConnections();

    setModal(true);
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    // No help/maximize; we’ll lock resizing in showDialog()
    setWindowFlags(windowFlags()
                   & ~Qt::WindowContextHelpButtonHint
                   & ~Qt::WindowMaximizeButtonHint);

    // Avoid any implicit size grip (artifact over buttons)
    setSizeGripEnabled(false);

    // Initial compute; final lock happens in showDialog()
    resizeToContent();
}

void PromptDialog::createWidgets() {
    iconDisplay = new QLabel;
    iconDisplay->setAlignment(Qt::AlignCenter);
    iconDisplay->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    messageLabel = new QLabel;
    messageLabel->setWordWrap(true);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    messageLabel->setContentsMargins(0, 0, 0, 0);
    messageLabel->setTextFormat(Qt::PlainText);   // preserve newlines

    detailLabel = new QLabel;
    detailLabel->setWordWrap(true);
    detailLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    detailLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    detailLabel->setTextFormat(Qt::PlainText);

    textScrollArea = new QScrollArea;
    textScrollArea->setFrameShape(QFrame::NoFrame);
    textScrollArea->setWidgetResizable(true);
    textScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textScrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);

    buttonBox = new QDialogButtonBox;
    mainLayout = new QVBoxLayout;
}

void PromptDialog::setupLayouts() {
    // Icon column
    auto *iconLayout = new QVBoxLayout;
    iconLayout->setContentsMargins(0, 0, 0, 0);
    iconLayout->setSpacing(0);
    iconLayout->addWidget(iconDisplay);
    iconLayout->addStretch();
    iconLayout->setAlignment(iconDisplay, Qt::AlignTop | Qt::AlignHCenter);

    auto *iconWrapper = new QWidget;
    iconWrapper->setLayout(iconLayout);

    // Text column
    auto *textLayout = new QVBoxLayout;
    textLayout->addWidget(messageLabel);
    textLayout->addWidget(detailLabel);
    textLayout->setSpacing(k_TEXT_SPACING);
    textLayout->setContentsMargins(0, 0, 0, 0);

    auto *textContainer = new QWidget;
    textContainer->setLayout(textLayout);

    textScrollArea->setWidget(textContainer);
    textScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Top row: icon + scrollable text
    auto *topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(k_OUTER_MARGIN / 2, 0, k_OUTER_MARGIN / 2, 0);
    topLayout->setSpacing(8);
    topLayout->addWidget(iconWrapper, 0);
    topLayout->addWidget(textScrollArea, 1);

    // Main layout
    mainLayout->addLayout(topLayout);
    mainLayout->addSpacing(k_SECTION_SPACING);
    mainLayout->addWidget(buttonBox);
    mainLayout->setContentsMargins(k_OUTER_MARGIN, k_OUTER_MARGIN, k_OUTER_MARGIN, k_OUTER_MARGIN);
    mainLayout->setSpacing(k_SECTION_SPACING);

    setLayout(mainLayout);
}

void PromptDialog::applyStyling() {
    setStyleSheet(DIALOG_STYLE);
    iconDisplay->setStyleSheet(ICON_LABEL_STYLE);
    messageLabel->setStyleSheet(TEXT_LABEL_STYLE);
    detailLabel->setStyleSheet(INFO_TEXT_LABEL_STYLE);

    if (textScrollArea) {
        textScrollArea->setStyleSheet(SCROLLAREA_STYLE);
        if (auto *vp = textScrollArea->viewport())
            vp->setAutoFillBackground(false);
    }

    for (QAbstractButton *btn : buttonBox->buttons()) {
        if (auto *push = qobject_cast<QPushButton*>(btn)) {
            push->setStyleSheet(BUTTON_STYLE);
        }
    }
}

void PromptDialog::configureConnections() {
    connect(buttonBox, &QDialogButtonBox::clicked, this, &PromptDialog::handleButtonClicked);
}

// ====== setters ======
void PromptDialog::setMessageText(const QString &text) {
    messageLabel->setText(text);
    resizeToContent();
}
void PromptDialog::setInformativeText(const QString &text) {
    detailLabel->setText(text);
    detailLabel->setVisible(!text.isEmpty());
    resizeToContent();
}
void PromptDialog::setIcon(Icon icon) {
    QPixmap original = iconPixmap(icon);
    QPixmap scaled = original.scaled(k_ICON_SIZE, k_ICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconDisplay->setPixmap(scaled);
}
void PromptDialog::setStandardButtons(Buttons buttons) {
    buttonBox->setStandardButtons(toStandardButtons(buttons));
    const auto btns = buttonBox->buttons();
    for (QAbstractButton* button : btns) {
        if (auto* pushBtn = qobject_cast<QPushButton*>(button)) {
            const QString text = pushBtn->text();
            Shared::UI::applyButtonTooltipAndCursor(pushBtn, text);
        }
    }
    resizeToContent();
}
void PromptDialog::setDefaultButton(Button button) {
    auto *defaultBtn = buttonBox->button(toStandardButton(button));
    if (defaultBtn) defaultBtn->setDefault(true);
}

// ====== clicks ======
void PromptDialog::handleButtonClicked(QAbstractButton *button) {
    switch (buttonBox->standardButton(button)) {
    case QDialogButtonBox::Ok:     userChoice = Ok;     break;
    case QDialogButtonBox::Cancel: userChoice = Cancel; break;
    case QDialogButtonBox::Yes:    userChoice = Yes;    break;
    case QDialogButtonBox::No:     userChoice = No;     break;
    default:                       userChoice = None;   break;
    }
    accept();
}

// ====== icons ======
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

// ====== exec helper (locks size) ======
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
        if (buttons.testFlag(Ok))        defaultButton = Ok;
        else if (buttons.testFlag(Yes))  defaultButton = Yes;
        else if (buttons.testFlag(No))   defaultButton = No;
        else if (buttons.testFlag(Cancel)) defaultButton = Cancel;
    }
    dialog.setDefaultButton(defaultButton);

    // Final sizing + centering
    dialog.resizeToContent();
    const QRect targetRect = parent
                                 ? parent->geometry()
                                 : QApplication::primaryScreen()->availableGeometry();
    dialog.move(targetRect.center() - dialog.rect().center());

    // 🔒 Make non-resizable after sizing/centering
    dialog.setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true); // Windows UX hint
    dialog.setFixedSize(dialog.size());

    dialog.exec();
    return dialog.userChoice;
}

// ====== sizing logic ======
int PromptDialog::computeTargetDialogWidth() const {
    const QRect avail = QApplication::primaryScreen()->availableGeometry();
    const int capW = int(avail.width() * 0.80);

    int targetW = qMax(k_DIALOG_MIN_WIDTH, k_DIALOG_BASE_WIDTH);

    auto joinText = [](const QLabel* lbl) -> QString {
        if (!lbl || !lbl->isVisible()) return {};
        return lbl->text();
    };
    const QString allText = joinText(messageLabel)
                            + (detailLabel && detailLabel->isVisible() && !detailLabel->text().isEmpty()
                                   ? "\n" + detailLabel->text() : QString());

    QFontMetrics fmMessage(messageLabel->font());
    QFontMetrics fmDetail(detailLabel->font());
    int widest = 0;
    const QString normText = QString(allText).replace('\t', "    ");
    const auto lines = normText.split('\n');
    for (const QString &line : lines) {
        widest = qMax(widest, fmMessage.horizontalAdvance(line));
        widest = qMax(widest, fmDetail.horizontalAdvance(line));
    }

    const int iconW = iconDisplay->sizeHint().width() + 16 /*spacing*/ + k_OUTER_MARGIN;
    const int paddings = k_OUTER_MARGIN * 2; // dialog left/right margins
    const int naturalDialogW = widest + iconW + paddings;

    targetW = qMax(targetW, naturalDialogW);
    targetW = qMin(targetW, capW);
    return targetW;
}

int PromptDialog::computeTextColumnWidth(int targetDialogW) const {
    const int iconW = iconDisplay->sizeHint().width();
    const int between = 8; // topLayout spacing
    const int sideMargins = (k_OUTER_MARGIN / 2) * 2; // topLayout L/R
    const int dialogMargins = k_OUTER_MARGIN * 2;
    const int contentRowW = targetDialogW - dialogMargins;
    const int textW = contentRowW - (iconW + between + sideMargins);
    return qMax(200, textW);
}

void PromptDialog::resizeToContent() {
    const QRect avail = QApplication::primaryScreen()->availableGeometry();
    const int capW  = int(avail.width()  * 0.80); // dialog max width
    const int capH  = int(avail.height() * 0.80); // dialog max height
    const int capTextH = int(avail.height() * 0.60); // scroll area max

    const int targetDialogW = computeTargetDialogWidth();

    if (auto *textContainer = textScrollArea->widget()) {
        const int textW = computeTextColumnWidth(targetDialogW);
        textContainer->setMinimumWidth(textW);
        textContainer->setMaximumWidth(textW);
        textContainer->adjustSize();

        const int contentH = textContainer->sizeHint().height();
        const int desiredTextH = qMin(contentH, capTextH);
        textScrollArea->setMaximumHeight(desiredTextH);
        textScrollArea->setMinimumHeight(qMin(desiredTextH, 420));
    }

    adjustSize();

    const int finalW = qMin(qMax(width(), targetDialogW), capW);
    const int finalH = qMin(height(), capH);
    resize(finalW, finalH);

    setMinimumSize(k_DIALOG_MIN_WIDTH, k_DIALOG_MIN_HEIGHT);
    setMaximumSize(capW, capH);
}
