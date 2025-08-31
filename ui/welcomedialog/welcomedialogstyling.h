#ifndef WELCOMEDIALOGSTYLING_H
#define WELCOMEDIALOGSTYLING_H

namespace WelcomeDialogStyling {
namespace Styles {

inline constexpr auto TEXT_LABEL_STYLE = R"(
    QLabel {
        font-size: 12pt;
        color: #EEE;
    }
)";

inline constexpr auto LOGO_LABEL_STYLE = R"(
    QLabel {
        margin-bottom: 12px;
    }
)";

} // namespace Styles
} // namespace WelcomeDialogStyling

#endif // WELCOMEDIALOGSTYLING_H
