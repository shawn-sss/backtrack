#ifndef TOOLBARMANAGERSTYLING_H
#define TOOLBARMANAGERSTYLING_H

// Qt includes
#include <QString>
#include <QSize>

namespace ToolbarStyling {

// Default icon size for toolbar buttons
inline constexpr QSize DEFAULT_ICON_SIZE(24, 24);

// Base style applied to QToolBar and QToolButtons
inline const QString BASE_STYLE = QStringLiteral(
    "QToolBar {"
    "    background-color: transparent;"
    "    border: none;"
    "}"
    "QToolBar QToolButton {"
    "    border-radius: 16px;"
    "    padding: 8px;"
    "    margin: 4px;"
    "    min-width: 40px;"
    "    min-height: 40px;"
    "    font-size: 14px;"
    "    border: none;"
    "}"
    );

} // namespace ToolbarStyling

#endif // TOOLBARMANAGERSTYLING_H
