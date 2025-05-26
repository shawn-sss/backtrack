#ifndef WELCOMEDIALOGSTYLING_H
#define WELCOMEDIALOGSTYLING_H

// Qt includes
#include <QString>

// Styling definitions for WelcomeDialog UI elements
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

inline constexpr auto BUTTON_STYLE = R"(
    QPushButton {
        background-color: #558B2F;
        color: white;
        border-radius: 5px;
        padding: 6px 14px;
    }
    QPushButton:hover {
        background-color: #33691E;
    }
    QPushButton:pressed {
        background-color: #1B5E20;
    }
)";

} // namespace Styles
} // namespace WelcomeDialogStyling

#endif // WELCOMEDIALOGSTYLING_H
