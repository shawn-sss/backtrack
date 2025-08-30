// filename: templatedialogconstants.h

#ifndef TEMPLATEDIALOGCONSTANTS_H
#define TEMPLATEDIALOGCONSTANTS_H

// C++ includes
#include <QString>

namespace TemplateDialogConstants {

inline constexpr const char* k_LOAD_BUTTON_TOOLTIP   = "Load the selected template";
inline constexpr const char* k_DELETE_BUTTON_TOOLTIP = "Delete the selected template";
inline constexpr const char* k_SAVE_BUTTON_TOOLTIP   = "Save a new template with current items";

inline constexpr int k_MIN_WIDTH  = 400;
inline constexpr int k_MIN_HEIGHT = 300;

inline constexpr const char* k_LOAD_BUTTON_TEXT   = "Load";
inline constexpr const char* k_DELETE_BUTTON_TEXT = "Delete";
inline constexpr const char* k_SAVE_BUTTON_TEXT   = "Save Current";

inline constexpr const char* k_WINDOW_TITLE          = "Manage Templates";
inline constexpr const char* k_DELETE_CONFIRM_TITLE  = "Delete Template";
inline constexpr const char* k_SAVE_PROMPT_TITLE     = "Save Template";
inline constexpr const char* k_SAVE_FAILED_TITLE     = "Save Failed";
inline constexpr const char* k_LOAD_NEW_PROMPT_TITLE = "Load New Template?";

inline constexpr const char* k_SAVE_PROMPT_MESSAGE = "Enter name for this Template:";
inline constexpr const char* k_SAVE_FAILED_MESSAGE = "Could not save Template.";

inline const QString k_DELETE_CONFIRM_MESSAGE =
    QStringLiteral("Are you sure you want to delete '%1'?");
inline const QString k_LOAD_NEW_PROMPT_MESSAGE =
    QStringLiteral("The Template '%1' was saved.\n\nWould you like to load it now?\n(This will clear your current staging list)");

} // namespace TemplateDialogConstants

#endif // TEMPLATEDIALOGCONSTANTS_H
