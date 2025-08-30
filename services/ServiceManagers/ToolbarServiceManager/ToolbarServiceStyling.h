#ifndef TOOLBARSERVICEMANAGERSTYLING_H
#define TOOLBARSERVICEMANAGERSTYLING_H

// Qt includes
#include <QString>
#include <QSize>

namespace ToolbarServiceStyling {

// Defaults
inline const QSize DefaultIconSize = QSize(24, 24);

// Styles
inline const QString BaseStyle = QStringLiteral(
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

}

#endif
