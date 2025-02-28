#include "customtitlebar.h"
#include "../../config/_constants.h"

#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QHBoxLayout>

// Constructor
CustomTitleBar::CustomTitleBar(QWidget *parent)
    : QWidget(parent),
    titleLabel(new QLabel(AppInfo::APP_DISPLAY_TITLE, this)),
    minimizeButton(createButton(Labels::WindowButtons::MINIMIZE, Styles::Button::MINIMIZE_STYLESHEET, Tooltips::WINDOW_MINIMIZE)),
    closeButton(createButton(Labels::WindowButtons::CLOSE, Styles::Button::CLOSE_STYLESHEET, Tooltips::WINDOW_CLOSE)) {

    // Enforce correct size and policies
    setFixedHeight(UISettings::TitleBar::HEIGHT);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Apply global stylesheet for the title bar
    setStyleSheet(Styles::TitleBar::STYLESHEET);

    // Setup internal layout and connections
    setupLayout();
}

// Sets up layout
void CustomTitleBar::setupLayout() {
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(5);

    // Title label styling
    titleLabel->setStyleSheet(Styles::TitleBar::LABEL_STYLESHEET);

    // Add widgets to the layout
    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(minimizeButton);
    layout->addWidget(closeButton);

    // Connect buttons to the title bar signals
    connect(minimizeButton, &QPushButton::clicked, this, &CustomTitleBar::minimizeRequested);
    connect(closeButton, &QPushButton::clicked, this, &CustomTitleBar::closeRequested);
}

// Creates button
QPushButton* CustomTitleBar::createButton(const QString &label, const QString &style, const QString &tooltip) {
    auto *button = new QPushButton(label, this);
    button->setStyleSheet(style);
    button->setToolTip(tooltip);

    // Consistent button size and cursor style
    button->setFixedSize(30, 25);
    button->setCursor(Qt::PointingHandCursor);

    return button;
}

// override minimumSizeHint to ensure layout behaves even if something external queries this
QSize CustomTitleBar::minimumSizeHint() const {
    return QSize(100, UISettings::TitleBar::HEIGHT);
}

// override sizeHint to give layouts a preferred size
QSize CustomTitleBar::sizeHint() const {
    return QSize(400, UISettings::TitleBar::HEIGHT);  // Width will expand as needed
}
