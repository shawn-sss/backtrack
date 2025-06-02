#ifndef ABOUTDIALOGSTYLING_H
#define ABOUTDIALOGSTYLING_H

// Qt includes
#include <QString>

namespace AboutDialogStyling {
namespace Styles {

// Style for the descriptive text label
inline constexpr auto TEXT_LABEL_STYLE = R"(
    QLabel {
        font-size: 12pt;
        color: #DDD;
    }
)";

// Style for the logo label
inline constexpr auto LOGO_LABEL_STYLE = R"(
    QLabel {
        margin-bottom: 10px;
    }
)";

} // namespace Styles
} // namespace AboutDialogStyling

#endif // ABOUTDIALOGSTYLING_H
