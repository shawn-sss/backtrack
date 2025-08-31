// Project includes
#include "helpdialog.h"
#include "helpdialogconstants.h"
#include "helpdialogstyling.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QTabWidget>
#include <QTextBrowser>
#include <QVBoxLayout>

namespace {

QString buildGettingStartedHtml() {
    using namespace HelpDialogConstants;
    return k_SECTION_GETTING_STARTED_TEMPLATE.arg(
        QObject::tr(k_GETTING_STARTED_WELCOME),
        QObject::tr(k_GETTING_STARTED_HOWTO),
        QObject::tr(k_GETTING_STARTED_STEP1),
        QObject::tr(k_GETTING_STARTED_STEP2),
        QObject::tr(k_GETTING_STARTED_STEP3),
        QObject::tr(k_GETTING_STARTED_STEP4),
        QObject::tr(k_GETTING_STARTED_DONE),
        QObject::tr(k_GETTING_STARTED_SUCCESS));
}

QString buildFeaturesHtml() {
    using namespace HelpDialogConstants;
    return k_SECTION_FEATURES_TEMPLATE.arg(
        QObject::tr(k_FEATURE_1_TITLE), QObject::tr(k_FEATURE_1_DESC),
        QObject::tr(k_FEATURE_2_TITLE), QObject::tr(k_FEATURE_2_DESC),
        QObject::tr(k_FEATURE_3_TITLE), QObject::tr(k_FEATURE_3_DESC),
        QObject::tr(k_FEATURE_4_TITLE), QObject::tr(k_FEATURE_4_DESC));
}

QString buildFaqHtml(const QString& appDataPath) {
    using namespace HelpDialogConstants;
    const QString safePath = appDataPath.toHtmlEscaped();
    return k_SECTION_FAQ_TEMPLATE.arg(
        QObject::tr(k_FAQ_Q),
        QObject::tr(k_FAQ_Q1),
        QObject::tr(k_FAQ_A),
        QObject::tr(k_FAQ_A1), safePath,
        QObject::tr(k_FAQ_Q),
        QObject::tr(k_FAQ_Q2),
        QObject::tr(k_FAQ_A),
        QObject::tr(k_FAQ_A2));
}

} // namespace

// Construct Help dialog window
HelpDialog::HelpDialog(QWidget* parent)
    : QDialog(parent),
    tabWidget(new QTabWidget(this)) {

    using namespace HelpDialogConstants;

    setWindowTitle(k_WINDOW_TITLE);
    resize(k_DIALOG_WIDTH, k_DIALOG_HEIGHT);

    const QString appDataPath = PathServiceManager::appDataRootDir();

    gettingStartedText = createTextBrowser(buildGettingStartedHtml());
    featuresText       = createTextBrowser(buildFeaturesHtml());
    faqText            = createTextBrowser(buildFaqHtml(appDataPath));

    tabWidget->addTab(gettingStartedText, k_TAB_GETTING_STARTED);
    tabWidget->addTab(featuresText,       k_TAB_FEATURES);
    tabWidget->addTab(faqText,            k_TAB_FAQ);
    tabWidget->setStyleSheet(HelpDialogStyling::Styles::TAB_WIDGET_STYLE);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    layout->setSpacing(k_MAIN_SPACING);
    layout->addWidget(tabWidget);
}

// Create a styled QTextBrowser
QTextBrowser* HelpDialog::createTextBrowser(const QString& html) {
    auto* browser = new QTextBrowser(this);
    browser->setHtml(html);
    browser->setOpenExternalLinks(true);
    browser->setTextInteractionFlags(Qt::TextBrowserInteraction);
    browser->setUndoRedoEnabled(false);
    browser->setStyleSheet(HelpDialogStyling::Styles::TEXT_BROWSER_STYLE);
    return browser;
}
