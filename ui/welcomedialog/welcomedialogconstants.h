#ifndef WELCOMEDIALOGCONSTANTS_H
#define WELCOMEDIALOGCONSTANTS_H

namespace WelcomeDialogConstants {

// Layout & size constants
constexpr int kLogoSize     = 150;
constexpr int kDialogWidth  = 500;
constexpr int kDialogHeight = 400;
constexpr int kTopMargin    = 20;

// Text strings
inline constexpr const char* k_WINDOW_TITLE = "Welcome";
inline constexpr const char* k_WELCOME_HTML =
    "<h2>Welcome to %1</h2>"
    "<p>Version: <b>%2</b></p>"
    "<p>Thank you for installing our application!</p>"
    "<p style='margin-top:%3px;'>Let's get you started...</p>";

// Resource paths
inline constexpr const char* k_LOGO_RESOURCE_PATH = ":/resources/icons/app_icon.png";

} // namespace WelcomeDialogConstants

#endif // WELCOMEDIALOGCONSTANTS_H
