#ifndef CUSTOMTITLEBARSTYLING_H
#define CUSTOMTITLEBARSTYLING_H

// Built-in Qt includes
#include <QString>

// TitleBarStyles - styles, tooltips, and UI settings for custom title bar
namespace TitleBarStyles {

// Base stylesheet for the title bar
inline const QString BASE_STYLESHEET = QStringLiteral(
    "background-color: #016394; "
    "color: white; "
    "border: none;"
    );

// Label stylesheet
inline const QString LABEL_STYLESHEET = QStringLiteral(
    "color: white; "
    "font-size: 24px; "
    "font-weight: bold;"
    );

// Base style shared by all buttons
inline const QString BUTTON_BASE_STYLE = QStringLiteral(
    "QPushButton { "
    "background: transparent; "
    "color: white; "
    "border: none; "
    "} "
    );

// Minimize button style
inline const QString MINIMIZE_BUTTON = BUTTON_BASE_STYLE +
                                       QStringLiteral("QPushButton:hover { background: rgba(255, 255, 255, 0.2); } "
                                                      "QPushButton:pressed { background: rgba(255, 255, 255, 0.3); }");

// Close button style
inline const QString CLOSE_BUTTON = BUTTON_BASE_STYLE +
                                    QStringLiteral("QPushButton:hover { background: red; color: white; } "
                                                   "QPushButton:pressed { background: darkred; color: white; }");

// Button tooltips
namespace Tooltips {
inline const QString WINDOW_MINIMIZE = QStringLiteral("Minimize Window");
inline const QString WINDOW_CLOSE = QStringLiteral("Close Application");
}

// Button labels
namespace WindowButtons {
inline const QString CLOSE = QStringLiteral("X");
inline const QString MINIMIZE = QStringLiteral("_");
}

// Title bar UI settings
namespace TitleBar {
inline constexpr int HEIGHT = 40;
}

// Button size constants
namespace Button {
inline constexpr int WIDTH = 30;
inline constexpr int HEIGHT = 25;
}

} // namespace TitleBarStyles

#endif // CUSTOMTITLEBARSTYLING_H
