#ifndef UIUTILSSERVICESTYLING_H
#define UIUTILSSERVICESTYLING_H

// Qt includes
#include <QString>

namespace Shared::UI::Styling::Buttons {

// Object name for SnapList reset button
inline constexpr auto k_SNAPLIST_RESET_BUTTON_OBJECT_NAME = "SnapListResetButton";

// Style sheet for SnapList reset button
inline constexpr auto k_SNAPLIST_RESET_BUTTON_STYLE = R"(
    QPushButton#SnapListResetButton {
        background-color: #a33;
        color: white;
        border: none;
        border-radius: 4px;
        font-size: 10px;
        min-width: 16px;
        min-height: 16px;
        max-width: 16px;
        max-height: 16px;
        padding: 0px;
        margin-left: 4px;
    }

    QPushButton#SnapListResetButton:hover {
        background-color: #c44;
    }
)";

} // namespace Shared::UI::Styling::Buttons

#endif // UIUTILSSERVICESTYLING_H
