#ifndef UIUTILSSERVICESTYLING_H
#define UIUTILSSERVICESTYLING_H

// Qt includes
#include <QString>

namespace Shared::UI::Styling::Buttons {

// Object name
inline const QString TemplateResetButtonObjectName = QStringLiteral("TemplateResetButton");

// Stylesheet
inline const QString TemplateResetButtonStyle = QStringLiteral(R"(
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

}

#endif
