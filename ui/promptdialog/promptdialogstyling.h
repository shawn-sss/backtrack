// filename: promptdialogstyling.h

#ifndef PROMPTDIALOGSTYLING_H
#define PROMPTDIALOGSTYLING_H

namespace PromptDialogStyling {
namespace Styles {

inline constexpr auto DIALOG_STYLE = R"(
    QDialog {
        border-radius: 8px;
    }
)";

inline constexpr auto ICON_LABEL_STYLE = R"(
    QLabel {
        padding-top: 8px;
        padding-left: 8px;
        padding-right: 8px;
    }
)";

inline constexpr auto TEXT_LABEL_STYLE = R"(
    QLabel {
        font-size: 11pt;
        font-weight: bold;
        margin-left: 1px;
        margin-bottom: 0px;
    }
)";

inline constexpr auto INFO_TEXT_LABEL_STYLE = R"(
    QLabel {
        font-size: 10pt;
        margin-top: 4px;
        margin-bottom: 0px;
    }
)";

inline constexpr auto SCROLLAREA_STYLE = R"(
    QScrollArea {
        background: transparent;
        border: none;
    }
    QScrollArea > QWidget {
        background: transparent;
    }
)";

inline constexpr auto BUTTON_STYLE = R"(
    QPushButton {
        border-radius: 6px;
        padding: 4px 12px;
        min-height: 26px;
        background-color: palette(button);
        color: palette(button-text);
        border: 1px solid palette(mid);
    }
    QPushButton:hover   { background-color: palette(midlight); }
    QPushButton:pressed { background-color: palette(mid); }
    QPushButton:default { font-weight: bold; border: 1px solid palette(highlight); }
    QPushButton:focus   { outline: 0; border: 1px solid palette(highlight); }
    QPushButton:disabled{
        color: palette(dark);
        background-color: palette(button);
        border: 1px solid palette(mid);
    }
)";

} // namespace Styles
} // namespace PromptDialogStyling

#endif // PROMPTDIALOGSTYLING_H
