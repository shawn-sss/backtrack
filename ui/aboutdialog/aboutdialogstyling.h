#ifndef ABOUTDIALOGSTYLING_H
#define ABOUTDIALOGSTYLING_H

// Styling for About dialog
namespace AboutDialogStyling {
namespace Styles {

inline constexpr auto TEXT_LABEL_STYLE = R"(
    QLabel {
        font-size: 12pt;
        color: #DDD;
    }
)";

inline constexpr auto LOGO_LABEL_STYLE = R"(
    QLabel {
        margin-bottom: 10px;
    }
)";

} // namespace Styles
} // namespace AboutDialogStyling

#endif // ABOUTDIALOGSTYLING_H
