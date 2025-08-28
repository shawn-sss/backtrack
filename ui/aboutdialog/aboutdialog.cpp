// Project includes
#include "aboutdialog.h"
#include "aboutdialogconstants.h"
#include "aboutdialogstyling.h"
#include "../../constants/app_info.h"
#include "../../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"

// Qt includes
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace {

// Build the HTML content for the About dialog
QString buildAboutHtmlText() {
    using namespace AboutDialogConstants;
    return kAboutHtmlTemplate.arg(
        App::Info::k_APP_NAME,
        QObject::tr("Version:"), App::Info::k_APP_VERSION,
        QObject::tr("Author:"),  App::Info::k_AUTHOR_NAME,
        QString::number(kTopMargin),
        App::Info::k_APP_ABOUT);
}

// Style a label for centered, wrapped rich/plain text
void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setTextFormat(richText ? Qt::RichText : Qt::PlainText);
}

} // namespace

// Constructor
AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent),
    logoLabel(new QLabel(this)),
    textLabel(new QLabel(this)),
    buttonBox(new QDialogButtonBox(QDialogButtonBox::Close, this)) {

    using namespace AboutDialogConstants;

    setWindowTitle(tr("%1").arg(kWindowTitle));
    resize(kDialogWidth, kDialogHeight);

    // Setup logo
    const QPixmap baseLogo(kLogoPath);
    if (!baseLogo.isNull()) {
        QPixmap scaled = baseLogo.scaled(
            kLogoSize, kLogoSize,
            kLogoAspectRatioMode,
            kLogoTransformMode);
        scaled.setDevicePixelRatio(devicePixelRatioF());
        logoLabel->setPixmap(scaled);
    }
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    logoLabel->setStyleSheet(AboutDialogStyling::Styles::LOGO_LABEL_STYLE);

    // Setup text
    textLabel->setText(buildAboutHtmlText());
    styleLabel(textLabel, true);
    textLabel->setStyleSheet(AboutDialogStyling::Styles::TEXT_LABEL_STYLE);
    textLabel->setOpenExternalLinks(true);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    // Setup button
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    for (QAbstractButton* b : buttonBox->buttons()) {
        if (auto* pushBtn = qobject_cast<QPushButton*>(b)) {
            Shared::UI::applyButtonTooltipAndCursor(pushBtn, kCloseButtonText);
        }
    }

    // Setup layout
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
    layout->addWidget(buttonBox);
}
