#include "customtitlebar.h"
#include "titlebarstyles.h"

#include "../../config/_constants.h"

#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QHBoxLayout>

// Button size constants
static constexpr int BUTTON_WIDTH = 30;
static constexpr int BUTTON_HEIGHT = 25;

// Constructor
CustomTitleBar::CustomTitleBar(QWidget *parent)
    : QWidget(parent),
    titleLabel(new QLabel(AppInfo::APP_DISPLAY_TITLE, this)),
    minimizeButton(createButton(TitleBarStyles::WindowButtons::MINIMIZE,
                                TitleBarStyles::MINIMIZE_BUTTON,
                                TitleBarStyles::Tooltips::WINDOW_MINIMIZE)),
    closeButton(createButton(TitleBarStyles::WindowButtons::CLOSE,
                             TitleBarStyles::CLOSE_BUTTON,
                             TitleBarStyles::Tooltips::WINDOW_CLOSE)) {
    setFixedHeight(TitleBarStyles::TitleBar::HEIGHT);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setStyleSheet(TitleBarStyles::BASE_STYLESHEET);
    setupLayout();
}

// Sets up layout for the title bar
void CustomTitleBar::setupLayout() {
    QPointer<QHBoxLayout> layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(5);

    titleLabel->setStyleSheet(TitleBarStyles::LABEL_STYLESHEET);

    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(minimizeButton);
    layout->addWidget(closeButton);

    connect(minimizeButton, &QPushButton::clicked, this, &CustomTitleBar::minimizeRequested);
    connect(closeButton, &QPushButton::clicked, this, &CustomTitleBar::closeRequested);
}

// Creates and returns a standardized button
QPushButton* CustomTitleBar::createButton(const QString &label, const QString &style, const QString &tooltip) {
    auto *button = new QPushButton(label, this);
    button->setStyleSheet(TitleBarStyles::BUTTON_BASE_STYLE + style);
    button->setToolTip(tooltip);
    button->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

// Returns minimum size hint for the title bar
QSize CustomTitleBar::minimumSizeHint() const {
    return QSize(100, TitleBarStyles::TitleBar::HEIGHT);
}

// Returns preferred size for the title bar
QSize CustomTitleBar::sizeHint() const {
    return QSize(parentWidget() ? parentWidget()->width() : 400, TitleBarStyles::TitleBar::HEIGHT);
}

// Sets up and returns a custom title bar for the given window
QPointer<CustomTitleBar> setupCustomTitleBar(QWidget *window, TitleBarMode mode) {
    if (!window) return nullptr;

    Qt::WindowFlags flags = Qt::FramelessWindowHint;
    if (mode == TitleBarMode::Dialog) {
        flags |= Qt::Dialog;
    }
    window->setWindowFlags(flags);

    if (auto existingTitleBar = window->findChild<CustomTitleBar *>()) {
        return existingTitleBar;
    }

    auto titleBar = new CustomTitleBar(window);
    titleBar->setFixedHeight(TitleBarStyles::TitleBar::HEIGHT);

    QObject::connect(titleBar, &CustomTitleBar::minimizeRequested, window, &QWidget::showMinimized);
    QObject::connect(titleBar, &CustomTitleBar::closeRequested, window, &QWidget::close);

    return titleBar;
}
