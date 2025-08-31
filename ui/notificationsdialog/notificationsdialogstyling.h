#ifndef NOTIFICATIONSDIALOGSTYLING_H
#define NOTIFICATIONSDIALOGSTYLING_H

namespace NotificationsDialogStyling {
namespace Styles {

inline constexpr auto k_LIST_WIDGET_STYLE = R"(
    QListWidget::item {
        padding: 4px 6px;
        font-size: 13px;
        font-family: 'Segoe UI', 'Arial', sans-serif;
        color: #dddddd;
    }
    QListWidget::item:selected {
        background-color: #2a3b4c;
    }
)";

inline constexpr auto k_BUTTON_STYLE = R"(
    QPushButton {
        padding: 6px 18px;
        font-weight: bold;
    }
)";

} // namespace Styles
} // namespace NotificationsDialogStyling

#endif // NOTIFICATIONSDIALOGSTYLING_H
