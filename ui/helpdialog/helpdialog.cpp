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

HelpDialog::HelpDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(HelpDialogConstants::kWindowTitle);
    resize(600, 400);

    tabWidget = new QTabWidget(this);
    gettingStartedText = new QTextBrowser(this);
    featuresText = new QTextBrowser(this);
    faqText = new QTextBrowser(this);

    gettingStartedText->setHtml(R"(
        <h2>Getting Started</h2>
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

    featuresText->setHtml(R"(
        <h2>Features</h2>
        <ul>
            <li><b>1:1 Backup:</b> Files are backed up exactly as they are, with no compression or alteration.</li>
            <li><b>Detailed Logging:</b> Logs include full metadata such as file names, timestamps, and more.</li>
            <li><b>Smart Filename Structure:</b> Each backup includes a customizable prefix and a timestamp suffix.</li>
            <li><b>Single Config File:</b> You only need to manage one settings file to keep preferences consistent.</li>
        </ul>
    )");

    QString appDataPath = ServiceDirector::getInstance().getAppInstallDirPublic();
    faqText->setHtml(QString(R"(
        <h2>Frequently Asked Questions</h2>
        <p><b>Q:</b> What traces does the app leave?<br>
        <b>A:</b> The app stores configuration and history data in the following location:<br>
        <code>%1</code></p>
        <p><b>Q:</b> How do I carry my settings to another machine?<br>
        <b>A:</b> Copy your <code>user_settings.json</code> file from the current system's data folder to the same location on the new machine.</p>
    )").arg(appDataPath));

    tabWidget->addTab(gettingStartedText, HelpDialogConstants::kTabGettingStarted);
    tabWidget->addTab(featuresText, HelpDialogConstants::kTabFeatures);
    tabWidget->addTab(faqText, HelpDialogConstants::kTabFAQ);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    HelpDialogStyling::applyCursors(tabWidget, buttonBox);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

HelpDialog::~HelpDialog() = default;
