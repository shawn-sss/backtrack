// filename: helpdialog.cpp

// Project includes
#include "helpdialog.h"
#include "HelpDialogConstants.h"
#include "HelpDialogStyling.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"

// Qt includes
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPushButton>

// C++ includes
#include <QString>

namespace {

QString buildGettingStartedHtml() {
    return HelpDialogConstants::kSectionGettingStartedTitle +
           QStringLiteral(
               R"(
               <p>%1</p>
               <h3>%2</h3>
               <ol>
                   <li>%3</li>
                   <li>%4</li>
                   <li>%5</li>
                   <li>%6</li>
               </ol>
               <h2 style='color:green;'>✅ %7 🎉</h2>
               <p>%8</p>
           )")
               .arg(QObject::tr("Welcome! This app helps you manage and protect your files with ease."),
                    QObject::tr("How to Create a Backup"),
                    QObject::tr("Select one or more drives, directories, or files."),
                    QObject::tr("Click the <b>Add to Backup</b> button."),
                    QObject::tr("Ensure all desired items are listed for backup."),
                    QObject::tr("Click the <b>Create Backup</b> button to begin."),
                    QObject::tr("DONE!"),
                    QObject::tr("You're all set — your backup has been created successfully!"));
}

QString buildFeaturesHtml() {
    return HelpDialogConstants::kSectionFeaturesTitle +
           QStringLiteral(
               R"(
               <ul>
                   <li><b>%1</b> %2</li>
                   <li><b>%3</b> %4</li>
                   <li><b>%5</b> %6</li>
                   <li><b>%7</b> %8</li>
               </ul>
           )")
               .arg(QObject::tr("1:1 Backup:"),
                    QObject::tr("Files are backed up exactly as they are, with no compression or alteration."),
                    QObject::tr("Detailed Logging:"),
                    QObject::tr("Logs include full metadata such as file names, timestamps, and more."),
                    QObject::tr("Smart Filename Structure:"),
                    QObject::tr("Each backup includes a customizable prefix and a timestamp suffix."),
                    QObject::tr("Single Config File:"),
                    QObject::tr("You only need to manage one settings file to keep preferences consistent."));
}

QString buildFaqHtml(const QString& appDataPath) {
    const QString safePath = appDataPath.toHtmlEscaped();
    return HelpDialogConstants::kSectionFAQTitle +
           QStringLiteral(
               R"(
               <p><b>%1</b> %2<br>
               <b>%3</b> %4<br>
               <code>%5</code></p>
               <p><b>%6</b> %7<br>
               <b>%8</b> %9 <code>user_settings.json</code> %10</p>
           )")
               .arg(QObject::tr("Q:"),
                    QObject::tr("What traces does the app leave?"),
                    QObject::tr("A:"),
                    QObject::tr("The app stores configuration and history data in the following location:"),
                    safePath,
                    QObject::tr("Q:"),
                    QObject::tr("How do I carry my settings to another machine?"),
                    QObject::tr("A:"),
                    QObject::tr("Copy your"),
                    QObject::tr("file from the current system's data folder to the same location on the new machine."));
}

} // namespace

// Creates a QTextBrowser with given HTML
QTextBrowser* HelpDialog::createTextBrowser(const QString& html) {
    auto* browser = new QTextBrowser(this);
    browser->setHtml(html);
    browser->setOpenExternalLinks(true);
    browser->setTextInteractionFlags(Qt::TextBrowserInteraction);
    browser->setUndoRedoEnabled(false);
    return browser;
}

// Constructs HelpDialog and initializes tabs, buttons, and styling
HelpDialog::HelpDialog(QWidget* parent)
    : QDialog(parent),
    tabWidget(new QTabWidget(this)) {

    setWindowTitle(HelpDialogConstants::kWindowTitle);
    resize(HelpDialogConstants::kDialogWidth, HelpDialogConstants::kDialogHeight);

    const QString appDataPath = PathServiceManager::appDataRootDir();

    gettingStartedText = createTextBrowser(buildGettingStartedHtml());
    featuresText       = createTextBrowser(buildFeaturesHtml());
    faqText            = createTextBrowser(buildFaqHtml(appDataPath));

    tabWidget->addTab(gettingStartedText, HelpDialogConstants::kTabGettingStarted);
    tabWidget->addTab(featuresText,       HelpDialogConstants::kTabFeatures);
    tabWidget->addTab(faqText,            HelpDialogConstants::kTabFAQ);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    for (auto* button : buttonBox->buttons()) {
        if (auto* pushButton = qobject_cast<QPushButton*>(button)) {
            Shared::UI::applyButtonTooltipAndCursor(pushButton, HelpDialogConstants::kCloseButtonText);
        }
    }

    Shared::UI::setTabWidgetCursorToPointer(tabWidget);
    tabWidget->setStyleSheet(HelpDialogStyling::Styles::TAB_WIDGET_STYLE);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);
    layout->addWidget(buttonBox);
}
