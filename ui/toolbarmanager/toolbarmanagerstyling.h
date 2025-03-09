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
    "background-color: transparent; "
    "border: none; "
    "padding: 5px;"
    );

// Minimal toolbar style
inline const QString MINIMAL_STYLE = QStringLiteral(
    "background-color: transparent; "
    "border: none;"
    );

} // namespace ToolbarStyles

#endif // TOOLBARMANAGERSTYLING_H
