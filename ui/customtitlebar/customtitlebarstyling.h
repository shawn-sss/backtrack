#ifndef CUSTOMTITLEBARSTYLING_H
#define CUSTOMTITLEBARSTYLING_H

// Built-in Qt includes
#include <QString>

// TitleBarStyles - styles, tooltips, and UI settings for custom title bar
namespace TitleBarStyles {

// Updated Title Bar Colors
inline const QString TITLEBAR_BACKGROUND_COLOR = QStringLiteral("#1E1F22");
inline const QString TITLE_TEXT_COLOR = QStringLiteral("#808080");

// Base stylesheet for the title bar
inline const QString BASE_STYLESHEET = QString(
                                           "background-color: %1; "
                                           "color: %2; "
                                           "border: none;"
                                           ).arg(TITLEBAR_BACKGROUND_COLOR, TITLE_TEXT_COLOR);

// Label (Title) Stylesheet
inline const QString LABEL_STYLESHEET = QString(
                                            "background-color: transparent; "
                                            "color: %1; "
                                            "font-size: 13px; "
                                            "font-weight: 800; "
                                            "letter-spacing: 0.3px; "
                                            "padding-left: 4px; "
                                            "padding-top: 1px; "
                                            ).arg(TITLE_TEXT_COLOR);

// Button base style
inline const QString BUTTON_BASE_STYLE = QStringLiteral(
                                             "QPushButton { "
                                             "background: transparent; "
                                             "color: %1; "
                                             "border: none; "
                                             "padding: 3px; "
                                             "} "
                                             ).arg(TITLE_TEXT_COLOR);

// Minimize button
inline const QString MINIMIZE_BUTTON = BUTTON_BASE_STYLE +
                                       QStringLiteral("QPushButton:hover { background: rgba(255, 255, 255, 0.1); } "
                                                      "QPushButton:pressed { background: rgba(255, 255, 255, 0.2); }");

// Maximize button
inline const QString MAXIMIZE_BUTTON = BUTTON_BASE_STYLE +
                                       QStringLiteral("QPushButton:hover { background: rgba(255, 255, 255, 0.1); } "
                                                      "QPushButton:pressed { background: rgba(255, 255, 255, 0.2); }");

// Close button (Red hover effect like Discord)
inline const QString CLOSE_BUTTON = BUTTON_BASE_STYLE +
                                    QStringLiteral("QPushButton:hover { background: #ED4245; color: white; } "
                                                   "QPushButton:pressed { background: #9B2C2F; color: white; }");

// Button tooltips
namespace Tooltips {
inline const QString WINDOW_MINIMIZE = QStringLiteral("Minimize");
inline const QString WINDOW_MAXIMIZE = QStringLiteral("Maximize");
inline const QString WINDOW_CLOSE = QStringLiteral("Close");
}

// Better symbols for buttons (closer to Discord’s proportions)
namespace WindowButtons {
inline const QString CLOSE = QStringLiteral("✕");
inline const QString MAXIMIZE = QStringLiteral("□");
inline const QString MINIMIZE = QStringLiteral("━");
}

// Title bar UI settings
namespace TitleBar {
inline constexpr int HEIGHT = 28;
}

// Button size constants
namespace Button {
inline constexpr int WIDTH = 24;
inline constexpr int HEIGHT = 24;
}

} // namespace TitleBarStyles

#endif // CUSTOMTITLEBARSTYLING_H
