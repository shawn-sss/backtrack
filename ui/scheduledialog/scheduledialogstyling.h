// filename: scheduledialogstyling.h

#ifndef SCHEDULEDIALOGSTYLING_H
#define SCHEDULEDIALOGSTYLING_H

namespace ScheduleDialogStyling {
namespace Styles {

inline constexpr auto HINT_LABEL_STYLE = R"(
    QLabel {
        font-size: 10pt;
        color: #DDDDDD;
        margin-bottom: 6px;
    }
)";

inline constexpr auto GROUPBOX_STYLE = R"(
    QGroupBox {
        font-weight: bold;
        margin-top: 8px;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 8px;
        padding: 0 4px;
    }
)";

} // namespace Styles
} // namespace ScheduleDialogStyling

#endif // SCHEDULEDIALOGSTYLING_H
