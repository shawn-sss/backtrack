#ifndef TOOLBARMANAGERSTYLING_H
#define TOOLBARMANAGERSTYLING_H

// Built-in Qt includes
#include <QSize>
#include <QString>

// Toolbar appearance styles
namespace ToolbarStyles {

// Default icon size
inline constexpr QSize DEFAULT_ICON_SIZE(24, 24);

// Default toolbar style
inline const QString DEFAULT_STYLE = QStringLiteral(
    "QToolBar {"
    "    background-color: transparent;"
    "    border: none;"
    "    padding: 2px;"
    "}"
    "QToolButton {"
    "    background-color: white;"
    "    border-radius: 16px;"
    "    padding: 8px;"
    "    margin: 4px;"
    "}"
    "QToolButton:hover {"
    "    background-color: #f0f0f0;"
    "}"
    );
}

#endif // TOOLBARMANAGERSTYLING_H
