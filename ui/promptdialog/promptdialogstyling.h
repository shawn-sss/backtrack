#ifndef PROMPTDIALOGSTYLING_H
#define PROMPTDIALOGSTYLING_H

namespace PromptDialogStyling {
namespace Styles {

// Dialog container style
inline constexpr auto DIALOG_STYLE = R"(
    QDialog {
        border-radius: 8px;
    }
)";

// Icon label style
inline constexpr auto ICON_LABEL_STYLE = R"(
    QLabel {
        padding-top: 8px;
        padding-left: 8px;
        padding-right: 8px;
    }
)";

// Title text label style
inline constexpr auto TEXT_LABEL_STYLE = R"(
    QLabel {
        font-size: 11pt;
        font-weight: bold;
        margin-left: 1px;
    }
)";

// Informational text label style
inline constexpr auto INFO_TEXT_LABEL_STYLE = R"(
    QLabel {
        font-size: 10pt;
        margin-top: 4px;
    }
)";

} // namespace Styles
} // namespace PromptDialogStyling

#endif // PROMPTDIALOGSTYLING_H
