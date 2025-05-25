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

// Style for the dialog buttons
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
} // namespace AboutDialogStyling

#endif // ABOUTDIALOGSTYLING_H
