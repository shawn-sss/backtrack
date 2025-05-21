#ifndef HELPDIALOGSTYLING_H
#define HELPDIALOGSTYLING_H

// Qt includes
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QTabBar>
#include <QTabWidget>
#include <QString>
#include "HelpDialogConstants.h"

namespace HelpDialogStyling {

inline void applyCursors(QTabWidget* tabWidget, QDialogButtonBox* buttonBox) {
    if (QTabBar* tabBar = tabWidget->findChild<QTabBar*>()) {
        tabBar->setCursor(Qt::PointingHandCursor);
    }

    const auto buttons = buttonBox->buttons();
    for (QAbstractButton* button : buttons) {
        button->setCursor(Qt::PointingHandCursor);
    }
}

inline QString buildGettingStartedHtml() {
    return QString(HelpDialogConstants::kSectionGettingStartedTitle) +
           QString::fromUtf8(R"(
               <p>Welcome! This app helps you manage and protect your files with ease.</p>
               <h3>How to Create a Backup</h3>
               <ol>
                   <li>Select one or more drives, directories, or files.</li>
                   <li>Click the <b>Add to Backup</b> button.</li>
                   <li>Ensure all desired items are listed for backup.</li>
                   <li>Click the <b>Create Backup</b> button to begin.</li>
               </ol>
               <h2 style='color:green;'>✅ DONE! 🎉</h2>
               <p>You're all set — your backup has been created successfully!</p>
           )");
}

inline QString buildFeaturesHtml() {
    return QString(HelpDialogConstants::kSectionFeaturesTitle) +
           QString::fromUtf8(R"(
               <ul>
                   <li><b>1:1 Backup:</b> Files are backed up exactly as they are, with no compression or alteration.</li>
                   <li><b>Detailed Logging:</b> Logs include full metadata such as file names, timestamps, and more.</li>
                   <li><b>Smart Filename Structure:</b> Each backup includes a customizable prefix and a timestamp suffix.</li>
                   <li><b>Single Config File:</b> You only need to manage one settings file to keep preferences consistent.</li>
               </ul>
           )");
}

inline QString buildFaqHtml(const QString& appDataPath) {
    return QString(HelpDialogConstants::kSectionFAQTitle) +
           QString(R"(
               <p><b>Q:</b> What traces does the app leave?<br>
               <b>A:</b> The app stores configuration and history data in the following location:<br>
               <code>%1</code></p>
               <p><b>Q:</b> How do I carry my settings to another machine?<br>
               <b>A:</b> Copy your <code>user_settings.json</code> file from the current system's data folder to the same location on the new machine.</p>
           )").arg(appDataPath);
}

} // namespace HelpDialogStyling

#endif // HELPDIALOGSTYLING_H
