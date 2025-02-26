#include "customtitlebar.h"
#include "../../config/_constants.h"

#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QHBoxLayout>

// Constructor
CustomTitleBar::CustomTitleBar(QWidget *parent)
    : QWidget(parent), titleLabel(new QLabel(AppInfo::APP_DISPLAY_TITLE, this)),
    minimizeButton(createButton(Labels::WindowButtons::MINIMIZE, Styles::Button::MINIMIZE_STYLESHEET, Tooltips::WINDOW_MINIMIZE)),
    closeButton(createButton(Labels::WindowButtons::CLOSE, Styles::Button::CLOSE_STYLESHEET, Tooltips::WINDOW_CLOSE)) {
    setFixedHeight(UISettings::TitleBar::HEIGHT);
    setStyleSheet(Styles::TitleBar::STYLESHEET);
    setupLayout();
}

// Sets up layout
void CustomTitleBar::setupLayout() {
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(5);

    titleLabel->setStyleSheet(Styles::TitleBar::LABEL_STYLESHEET);

    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(minimizeButton);
    layout->addWidget(closeButton);

    connect(minimizeButton, &QPushButton::clicked, this, &CustomTitleBar::minimizeRequested);
    connect(closeButton, &QPushButton::clicked, this, &CustomTitleBar::closeRequested);
}

// Creates button
QPushButton* CustomTitleBar::createButton(const QString &label, const QString &style, const QString &tooltip) {
    auto *button = new QPushButton(label, this);
    button->setStyleSheet(style);
    button->setToolTip(tooltip);
    button->setFixedSize(30, 25);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}
