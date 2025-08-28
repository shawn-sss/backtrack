// filename: welcomedialogconstants.h

#ifndef WELCOMEDIALOGCONSTANTS_H
#define WELCOMEDIALOGCONSTANTS_H

// C++ includes
#include <QString>

namespace WelcomeDialogConstants {

constexpr int kLogoSize     = 150;
constexpr int kDialogWidth  = 500;
constexpr int kDialogHeight = 400;
constexpr int kTopMargin    = 20;

inline constexpr const char* k_WINDOW_TITLE = "Welcome";
inline const QString k_WELCOME_HTML = QStringLiteral(
    "<h2>Welcome to %1</h2>"
    "<p>Version: <b>%2</b></p>"
    "<p>Thank you for installing our application!</p>"
    "<p style='margin-top:%3px;'>Let's get you started...</p>");

inline constexpr const char* k_LOGO_RESOURCE_PATH = ":/resources/icons/app_icon.png";
inline constexpr const char* k_OK_BUTTON_TOOLTIP  = "Close the dialog";

} // namespace WelcomeDialogConstants

#endif // WELCOMEDIALOGCONSTANTS_H
