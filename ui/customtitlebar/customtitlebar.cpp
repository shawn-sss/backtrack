// Project includes same directory
#include "customtitlebar.h"
#include "customtitlebarstyling.h"

// Project includes different directory
#include "../../config/_constants.h"
#include "../../core/utils/common_utils/utils.h"

// Built-in Qt includes
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>

// Constructor
CustomTitleBar::CustomTitleBar(QWidget *parent)
    : QWidget(parent),
    titleLabel(new QLabel(AppInfo::APP_DISPLAY_TITLE, this)),
    minimizeButton(createButton(TitleBarStyles::WindowButtons::MINIMIZE,
                                TitleBarStyles::MINIMIZE_BUTTON,
                                TitleBarStyles::Tooltips::WINDOW_MINIMIZE)),
    maximizeButton(createButton(TitleBarStyles::WindowButtons::MAXIMIZE,
                                TitleBarStyles::MAXIMIZE_BUTTON,
                                TitleBarStyles::Tooltips::WINDOW_MAXIMIZE)),
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
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    titleLabel->setStyleSheet(TitleBarStyles::LABEL_STYLESHEET);

    QVBoxLayout *titleWrapper = new QVBoxLayout();
    titleWrapper->setContentsMargins(0, 0, 0, 0);
    titleWrapper->setSpacing(0);
    titleWrapper->addWidget(titleLabel, 0, Qt::AlignTop | Qt::AlignLeft);

    QHBoxLayout *buttonWrapper = new QHBoxLayout();
    buttonWrapper->setContentsMargins(0, 0, 0, 0);
    buttonWrapper->setSpacing(0);
    buttonWrapper->addWidget(minimizeButton, 0, Qt::AlignTop | Qt::AlignRight);
    buttonWrapper->addWidget(maximizeButton, 0, Qt::AlignTop | Qt::AlignRight);
    buttonWrapper->addWidget(closeButton, 0, Qt::AlignTop | Qt::AlignRight);

    mainLayout->addLayout(titleWrapper);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonWrapper);

    connect(minimizeButton, &QPushButton::clicked, this, &CustomTitleBar::minimizeRequested);
    connect(maximizeButton, &QPushButton::clicked, this, &CustomTitleBar::maximizeRequested);
    connect(closeButton, &QPushButton::clicked, this, &CustomTitleBar::closeRequested);
}

// ## UI Appearance & Behavior ##

// Handles painting the custom title bar
void CustomTitleBar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor(TitleBarStyles::TITLEBAR_BACKGROUND_COLOR));
    QWidget::paintEvent(event);
}

// Returns minimum size hint for the title bar
QSize CustomTitleBar::minimumSizeHint() const {
    return QSize(100, TitleBarStyles::TitleBar::HEIGHT);
}

// Returns preferred size for the title bar
QSize CustomTitleBar::sizeHint() const {
    return QSize(parentWidget() ? (parentWidget()->width() > 0 ? parentWidget()->width() : 400) : 400, TitleBarStyles::TitleBar::HEIGHT);
}

// ## Button & Interaction Management ##

// Creates and returns a standardized button
QPushButton* CustomTitleBar::createButton(const QString &label, const QString &fullStyle, const QString &tooltip) {
    auto *button = new QPushButton(label, this);
    button->setStyleSheet(fullStyle);
    button->setToolTip(tooltip);
    button->setFixedSize(TitleBarStyles::Button::WIDTH, TitleBarStyles::Button::HEIGHT);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

// ## Window Management ##

// Sets up and returns a custom title bar for the given window
QPointer<CustomTitleBar> setupCustomTitleBar(QWidget *window, TitleBarMode mode) {
    if (!window) return nullptr;

    Qt::WindowFlags flags = Qt::FramelessWindowHint;
    if (mode == TitleBarMode::Dialog) {
        flags |= Qt::Dialog;
    }
    window->setWindowFlags(window->windowFlags() | flags);

    if (auto existingTitleBar = window->findChild<CustomTitleBar *>()) {
        return existingTitleBar;
    }

    auto *titleBar = new CustomTitleBar(window);

    QObject::connect(titleBar, &CustomTitleBar::minimizeRequested, window, &QWidget::showMinimized);
    QObject::connect(titleBar, &CustomTitleBar::maximizeRequested, window, [window]() {
        if (window->isMaximized()) {
            window->showNormal();
        } else {
            window->showMaximized();
        }
    });
    QObject::connect(titleBar, &CustomTitleBar::closeRequested, window, &QWidget::close);

    return titleBar;
}

// ## Handles mouse interactions for the custom title bar ##

// Handles the mouse press event to initiate dragging
void CustomTitleBar::mousePressEvent(QMouseEvent *event) {
    Utils::UI::handleMousePress(window(), event, dragging, lastMousePosition);
}

// Handles the mouse move event to update the window position while dragging
void CustomTitleBar::mouseMoveEvent(QMouseEvent *event) {
    Utils::UI::handleMouseMove(window(), event, dragging, lastMousePosition);
}

// Handles the mouse release event to stop dragging
void CustomTitleBar::mouseReleaseEvent(QMouseEvent *event) {
    Utils::UI::handleMouseRelease(event, dragging);
}

// Double-click event to maximize/restore window
void CustomTitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit maximizeRequested();
    }
}
