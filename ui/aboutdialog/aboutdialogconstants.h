#ifndef ABOUTDIALOGCONSTANTS_H
#define ABOUTDIALOGCONSTANTS_H

// Qt includes
#include <Qt>
#include <QString>

// Constants used in About dialog
namespace AboutDialogConstants {

constexpr int kLogoSize     = 150;
constexpr int kDialogWidth  = 500;
constexpr int kDialogHeight = 400;
constexpr int kTopMargin    = 15;

inline const QString kWindowTitle     = QStringLiteral("About");
inline const QString kLogoPath        = QStringLiteral(":/resources/icons/app_icon.png");
inline const QString kCloseButtonText = QStringLiteral("Close");

constexpr auto kLogoAspectRatioMode = Qt::KeepAspectRatio;
constexpr auto kLogoTransformMode   = Qt::SmoothTransformation;

inline const QString kAboutHtmlTemplate = QStringLiteral(
    "<h2>%1</h2>"
    "<p>%2 <b>%3</b></p>"
    "<p>%4 <b>%5</b></p>"
    "<p style='margin-top:%6px;'>%7</p>");

} // namespace AboutDialogConstants

#endif // ABOUTDIALOGCONSTANTS_H
