#ifndef ABOUTDIALOGCONSTANTS_H
#define ABOUTDIALOGCONSTANTS_H

// Qt includes
#include <Qt>
#include <QString>

namespace AboutDialogConstants {

// Dimensions
constexpr int k_LOGO_SIZE     = 150;
constexpr int k_DIALOG_WIDTH  = 500;
constexpr int k_DIALOG_HEIGHT = 400;
constexpr int k_TOP_MARGIN    = 15;
constexpr int k_MAIN_MARGIN   = 10;
constexpr int k_MAIN_SPACING  = 10;

// Titles & labels
inline constexpr const char* k_WINDOW_TITLE      = "About";
inline constexpr const char* k_LOGO_PATH         = ":/resources/icons/app_icon.png";
inline constexpr const char* k_CLOSE_BUTTON_TEXT = "Close";
inline constexpr const char* k_LABEL_VERSION     = "Version:";
inline constexpr const char* k_LABEL_AUTHOR      = "Author:";

// Logo rendering modes
constexpr auto k_LOGO_ASPECT_RATIO_MODE = Qt::KeepAspectRatio;
constexpr auto k_LOGO_TRANSFORM_MODE    = Qt::SmoothTransformation;

// HTML template
inline const QString k_ABOUT_HTML_TEMPLATE = QStringLiteral(
    "<h2>%1</h2>"
    "<p>%2 <b>%3</b></p>"
    "<p>%4 <b>%5</b></p>"
    "<p style='margin-top:%6px;'>%7</p>");

} // namespace AboutDialogConstants

#endif // ABOUTDIALOGCONSTANTS_H
