#ifndef UIUTILSSERVICESTYLING_H
#define UIUTILSSERVICESTYLING_H

// Qt includes
#include <QString>

namespace Shared::UI::Styling::Buttons {

// Object name for Template reset button
inline const QString k_TEMPLATE_RESET_BUTTON_OBJECT_NAME = QStringLiteral("TemplateResetButton");

// Style sheet for Template reset button
inline const QString k_TEMPLATE_RESET_BUTTON_STYLE = QStringLiteral(R"(
    #TemplateResetButton {
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

    #TemplateResetButton:hover {
        background-color: #c44;
    }
)");


} // namespace Shared::UI::Styling::Buttons

#endif // UIUTILSSERVICESTYLING_H
