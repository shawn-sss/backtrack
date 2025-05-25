#ifndef HELPDIALOGSTYLING_H
#define HELPDIALOGSTYLING_H

// Qt includes
#include <QString>

namespace HelpDialogStyling {
namespace Styles {

// Style for the tab widget
inline constexpr auto TAB_WIDGET_STYLE = R"(
    QTabWidget::pane {
        border: 1px solid #888;
        padding: 4px;
    }

    QTabBar::tab {
        background: #333;
        color: #DDD;
        padding: 6px 12px;
        border-top-left-radius: 4px;
        border-top-right-radius: 4px;
    }

    QTabBar::tab:selected {
        background: #555;
        color: white;
    }

    QTabBar::tab:hover {
        background: #444;
    }
)";

// Style for the dialog's close button
inline constexpr auto BUTTON_STYLE = R"(
    QPushButton {
        background-color: #444;
        color: white;
        border-radius: 5px;
        padding: 4px 10px;
    }
    QPushButton:hover {
        background-color: #666;
    }
    QPushButton:pressed {
        background-color: #222;
    }
)";

} // namespace Styles
} // namespace HelpDialogStyling

#endif // HELPDIALOGSTYLING_H
