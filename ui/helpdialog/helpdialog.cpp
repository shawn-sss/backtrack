// Project includes
#include "helpdialog.h"
#include "HelpDialogConstants.h"
#include "HelpDialogStyling.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QTextBrowser>
#include <QVBoxLayout>

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

    QString appDataPath = PathServiceManager::appDataRootDir();
    gettingStartedText = createTextBrowser(this, HelpDialogStyling::buildGettingStartedHtml());
    featuresText = createTextBrowser(this, HelpDialogStyling::buildFeaturesHtml());
    faqText = createTextBrowser(this, HelpDialogStyling::buildFaqHtml(appDataPath));

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
