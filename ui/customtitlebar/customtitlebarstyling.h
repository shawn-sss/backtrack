#ifndef CUSTOMTITLEBARSTYLING_H
#define CUSTOMTITLEBARSTYLING_H

#include <QString>

// TitleBarStyles - styles, tooltips, and UI settings for custom title bar
namespace TitleBarStyles {

// Base stylesheet for the title bar
const QString BASE_STYLESHEET =
    "background-color: #016394; "
    "color: white; "
    "border: none;";

// Label stylesheet
const QString LABEL_STYLESHEET =
    "color: white; "
    "font-size: 24px; "
    "font-weight: bold;";

// Base style shared by all buttons
const QString BUTTON_BASE_STYLE =
    "QPushButton { "
    "background: transparent; "
    "color: white; "
    "border: none; "
    "} ";

// Minimize button style
const QString MINIMIZE_BUTTON = BUTTON_BASE_STYLE +
                                "QPushButton:hover { background: rgba(255, 255, 255, 0.2); } "
                                "QPushButton:pressed { background: rgba(255, 255, 255, 0.3); }";

// Close button style
const QString CLOSE_BUTTON = BUTTON_BASE_STYLE +
                             "QPushButton:hover { background: red; color: white; } "
                             "QPushButton:pressed { background: darkred; color: white; }";

// Button tooltips
namespace Tooltips {
constexpr auto WINDOW_MINIMIZE = "Minimize Window";
constexpr auto WINDOW_CLOSE = "Close Application";
}

// Button labels
namespace WindowButtons {
constexpr auto CLOSE = "X";
constexpr auto MINIMIZE = "_";
}

// Title bar UI settings
namespace TitleBar {
constexpr int HEIGHT = 40;
}

// Button size constants - moved from customtitlebar.cpp
namespace Button {
constexpr int WIDTH = 30;
constexpr int HEIGHT = 25;
}

} // namespace TitleBarStyles

#endif // CUSTOMTITLEBARSTYLING_H
