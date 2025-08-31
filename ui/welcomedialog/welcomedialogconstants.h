#ifndef WELCOMEDIALOGCONSTANTS_H
#define WELCOMEDIALOGCONSTANTS_H

namespace WelcomeDialogConstants {

constexpr int k_DIALOG_WIDTH  = 500;
constexpr int k_DIALOG_HEIGHT = 350;
constexpr int k_LOGO_SIZE     = 150;
constexpr int k_TOP_MARGIN    = 20;
constexpr int k_MAIN_MARGIN   = 12;
constexpr int k_MAIN_SPACING  = 10;

inline constexpr const char* k_WINDOW_TITLE       = "Welcome";
inline constexpr const char* k_LOGO_RESOURCE_PATH = ":/resources/icons/app_icon.png";

inline constexpr const char* k_WELCOME_HTML =
    "<h2>Welcome to %1</h2>"
    "<p>Version: <b>%2</b></p>"
    "<p>Thank you for installing our application!</p>"
    "<p style='margin-top:%3px;'>Let's get you started...</p>";

} // namespace WelcomeDialogConstants

#endif // WELCOMEDIALOGCONSTANTS_H
