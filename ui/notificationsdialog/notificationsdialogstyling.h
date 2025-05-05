#ifndef NOTIFICATIONSDIALOGSTYLING_H
#define NOTIFICATIONSDIALOGSTYLING_H

// Qt includes
#include <QString>

namespace NotificationsDialogStyling {

// Style applied to QListWidget items for consistent padding, font, and selection appearance
inline const QString LIST_WIDGET_STYLE = QStringLiteral(
    "QListWidget::item {"
    "  margin: 0px;"
    "  padding: 2px 6px;"
    "  border: none;"
    "  font-size: 13px;"
    "  font-family: 'Segoe UI', 'Arial', sans-serif;"
    "  color: #dddddd;"
    "}"
    "QListWidget::item:selected {"
    "  background-color: #2a3b4c;"
    "}");

} // namespace NotificationsDialogStyling

#endif // NOTIFICATIONSDIALOGSTYLING_H
