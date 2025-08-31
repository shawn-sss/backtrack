#ifndef SETTINGSDIALOGSTYLING_H
#define SETTINGSDIALOGSTYLING_H

namespace SettingsDialogStyling {
namespace Styles {

inline constexpr auto CATEGORY_LIST_STYLE = R"(
    QListWidget {
        outline: none;
    }
    QListWidget::item:selected {
        background: #3a3a3a;
        color: white;
    }
)";

inline constexpr auto DESCRIPTION_LABEL_STYLE = "color: gray; font-size: 11px;";

} // namespace Styles
} // namespace SettingsDialogStyling

#endif
