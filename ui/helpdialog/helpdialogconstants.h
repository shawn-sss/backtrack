#ifndef HELPDIALOGCONSTANTS_H
#define HELPDIALOGCONSTANTS_H

// Qt includes
#include <QString>

namespace HelpDialogConstants {

// Dialog dimensions
constexpr int k_DIALOG_WIDTH  = 600;
constexpr int k_DIALOG_HEIGHT = 400;
constexpr int k_MAIN_MARGIN   = 10;
constexpr int k_MAIN_SPACING  = 10;

// Window title
inline constexpr const char* k_WINDOW_TITLE = "Help & Support";

// Tab titles
inline const QString k_TAB_GETTING_STARTED = QStringLiteral("Getting Started");
inline const QString k_TAB_FEATURES        = QStringLiteral("Features");
inline const QString k_TAB_FAQ             = QStringLiteral("FAQ");

// Section templates
inline const QString k_SECTION_GETTING_STARTED_TEMPLATE = QStringLiteral(
    "<h2>Getting Started</h2>"
    "<p>%1</p>"
    "<h3>%2</h3>"
    "<ol>"
    "<li>%3</li>"
    "<li>%4</li>"
    "<li>%5</li>"
    "<li>%6</li>"
    "</ol>"
    "<h2 style='color:green;'>✅ %7 🎉</h2>"
    "<p>%8</p>");

inline const QString k_SECTION_FEATURES_TEMPLATE = QStringLiteral(
    "<h2>Features</h2>"
    "<ul>"
    "<li><b>%1</b> %2</li>"
    "<li><b>%3</b> %4</li>"
    "<li><b>%5</b> %6</li>"
    "<li><b>%7</b> %8</li>"
    "</ul>");

inline const QString k_SECTION_FAQ_TEMPLATE = QStringLiteral(
    "<h2>Frequently Asked Questions</h2>"
    "<p><b>%1</b> %2<br>"
    "<b>%3</b> %4<br>"
    "<code>%5</code></p>"
    "<p><b>%6</b> %7<br>"
    "<b>%8</b> %9 <code>user_settings.json</code></p>");

// Getting started strings
inline constexpr const char* k_GETTING_STARTED_WELCOME  = "Welcome! This app helps you manage and protect your files with ease.";
inline constexpr const char* k_GETTING_STARTED_HOWTO    = "How to Create a Backup";
inline constexpr const char* k_GETTING_STARTED_STEP1    = "Select one or more drives, directories, or files.";
inline constexpr const char* k_GETTING_STARTED_STEP2    = "Click the <b>Add to Backup</b> button.";
inline constexpr const char* k_GETTING_STARTED_STEP3    = "Ensure all desired items are listed for backup.";
inline constexpr const char* k_GETTING_STARTED_STEP4    = "Click the <b>Create Backup</b> button to begin.";
inline constexpr const char* k_GETTING_STARTED_DONE     = "DONE!";
inline constexpr const char* k_GETTING_STARTED_SUCCESS  = "You're all set — your backup has been created successfully!";

// Feature strings
inline constexpr const char* k_FEATURE_1_TITLE = "1:1 Backup:";
inline constexpr const char* k_FEATURE_1_DESC  = "Files are backed up exactly as they are, with no compression or alteration.";
inline constexpr const char* k_FEATURE_2_TITLE = "Detailed Logging:";
inline constexpr const char* k_FEATURE_2_DESC  = "Logs include full metadata such as file names, timestamps, and more.";
inline constexpr const char* k_FEATURE_3_TITLE = "Smart Filename Structure:";
inline constexpr const char* k_FEATURE_3_DESC  = "Each backup includes a customizable prefix and a timestamp suffix.";
inline constexpr const char* k_FEATURE_4_TITLE = "Single Config File:";
inline constexpr const char* k_FEATURE_4_DESC  = "You only need to manage one settings file to keep preferences consistent.";

// FAQ strings
inline constexpr const char* k_FAQ_Q   = "Q:";
inline constexpr const char* k_FAQ_A   = "A:";
inline constexpr const char* k_FAQ_Q1  = "What traces does the app leave?";
inline constexpr const char* k_FAQ_A1  = "The app stores configuration and history data in the following location:";
inline constexpr const char* k_FAQ_Q2  = "How do I carry my settings to another machine?";
inline constexpr const char* k_FAQ_A2  = "Copy your";

} // namespace HelpDialogConstants

#endif // HELPDIALOGCONSTANTS_H
