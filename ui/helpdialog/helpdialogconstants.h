// filename: helpdialogconstants.h

#ifndef HELPDIALOGCONSTANTS_H
#define HELPDIALOGCONSTANTS_H

// C++ includes
#include <QString>

namespace HelpDialogConstants {

inline const QString kWindowTitle = QStringLiteral("Help & Support");

constexpr int kDialogWidth  = 600;
constexpr int kDialogHeight = 400;

inline const QString kTabGettingStarted = QStringLiteral("Getting Started");
inline const QString kTabFeatures       = QStringLiteral("Features");
inline const QString kTabFAQ            = QStringLiteral("FAQ");

inline const QString kSectionGettingStartedTitle = QStringLiteral("<h2>Getting Started</h2>");
inline const QString kSectionFeaturesTitle       = QStringLiteral("<h2>Features</h2>");
inline const QString kSectionFAQTitle            = QStringLiteral("<h2>Frequently Asked Questions</h2>");

inline const QString kCloseButtonText = QStringLiteral("Close");

} // namespace HelpDialogConstants

#endif // HELPDIALOGCONSTANTS_H
