// Project includes
#include "helpdialog.h"
#include "HelpDialogConstants.h"
#include "HelpDialogStyling.h"
#include "../../../../services/ServiceDirector/ServiceDirector.h"

// Qt includes
#include <QTabWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QDialogButtonBox>

namespace {
QTextBrowser* createTextBrowser(QWidget* parent, const QString& html) {
    auto* browser = new QTextBrowser(parent);
    browser->setHtml(html);
    return browser;
}
}

// Constructs the Help dialog with tabbed HTML content and a close button
HelpDialog::HelpDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(HelpDialogConstants::kWindowTitle);
    resize(HelpDialogConstants::kDialogWidth, HelpDialogConstants::kDialogHeight);

    tabWidget = new QTabWidget(this);

    QString gettingStartedHtml =
        QString(HelpDialogConstants::kSectionGettingStartedTitle) +
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

    QString featuresHtml =
        QString(HelpDialogConstants::kSectionFeaturesTitle) +
        QString::fromUtf8(R"(
            <ul>
                <li><b>1:1 Backup:</b> Files are backed up exactly as they are, with no compression or alteration.</li>
                <li><b>Detailed Logging:</b> Logs include full metadata such as file names, timestamps, and more.</li>
                <li><b>Smart Filename Structure:</b> Each backup includes a customizable prefix and a timestamp suffix.</li>
                <li><b>Single Config File:</b> You only need to manage one settings file to keep preferences consistent.</li>
            </ul>
        )");

    QString appDataPath = ServiceDirector::getInstance().getAppInstallDir();
    QString faqHtml =
        QString(HelpDialogConstants::kSectionFAQTitle) +
        QString(R"(
            <p><b>Q:</b> What traces does the app leave?<br>
            <b>A:</b> The app stores configuration and history data in the following location:<br>
            <code>%1</code></p>
            <p><b>Q:</b> How do I carry my settings to another machine?<br>
            <b>A:</b> Copy your <code>user_settings.json</code> file from the current system's data folder to the same location on the new machine.</p>
        )").arg(appDataPath);

    gettingStartedText = createTextBrowser(this, gettingStartedHtml);
    featuresText = createTextBrowser(this, featuresHtml);
    faqText = createTextBrowser(this, faqHtml);

    tabWidget->addTab(gettingStartedText, HelpDialogConstants::kTabGettingStarted);
    tabWidget->addTab(featuresText, HelpDialogConstants::kTabFeatures);
    tabWidget->addTab(faqText, HelpDialogConstants::kTabFAQ);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    HelpDialogStyling::applyCursors(tabWidget, buttonBox);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
    layout->addWidget(buttonBox);
}
