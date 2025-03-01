#ifndef TOOLBARMANAGERSTYLING_H
#define TOOLBARMANAGERSTYLING_H

#include <QString>
#include <QSize>

// Icon size styles
namespace IconStyles {
inline constexpr QSize DEFAULT_ICON_SIZE(24, 24);
}

// Toolbar appearance styles
namespace ToolbarStyles {
inline constexpr auto DEFAULT_STYLE =
    "background-color: transparent; "
    "border: none; "
    "padding: 5px;";

inline constexpr auto MINIMAL_STYLE =
    "background-color: transparent; "
    "border: none;";

inline constexpr QSize DEFAULT_ICON_SIZE(24, 24);
}

#endif // TOOLBARMANAGERSTYLING_H
