#ifndef TEMPLATEDIALOGSTYLING_H
#define TEMPLATEDIALOGSTYLING_H

namespace TemplateDialogStyling {
namespace Styles {

inline constexpr auto k_DEFAULT_BUTTON_GOLD = R"(
    QToolButton {
        color: gold;
    }
)";

inline constexpr auto k_DEFAULT_BUTTON_GRAY = R"(
    QToolButton {
        color: gray;
    }
)";

inline constexpr auto k_TABLE_STYLE = R"(
    QTableWidget {
        gridline-color: #ccc;
        selection-background-color: #eef;
    }
)";

inline constexpr int k_ACTION_LAYOUT_MARGIN = 0;
inline constexpr int k_ACTION_LAYOUT_SPACING = 2;

} // namespace Styles
} // namespace TemplateDialogStyling

#endif // TEMPLATEDIALOGSTYLING_H
