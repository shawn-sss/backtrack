// Project includes
#include "aboutdialog.h"
#include "aboutdialogconstants.h"
#include "aboutdialogstyling.h"
#include "../../constants/app_info.h"

// Qt includes
#include <QLabel>
#include <QPixmap>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace {

QString buildAboutHtmlText() {
    using namespace AboutDialogConstants;
    return k_ABOUT_HTML_TEMPLATE.arg(
        App::Info::k_APP_NAME,
        QObject::tr(k_LABEL_VERSION), App::Info::k_APP_VERSION,
        QObject::tr(k_LABEL_AUTHOR),  App::Info::k_AUTHOR_NAME,
        QString::number(k_TOP_MARGIN),
        App::Info::k_APP_ABOUT);
}

void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setTextFormat(richText ? Qt::RichText : Qt::PlainText);
}

} // namespace

// Construct About dialog window
AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent),
    logoLabel(new QLabel(this)),
    textLabel(new QLabel(this)) {

    using namespace AboutDialogConstants;

    setWindowTitle(tr(k_WINDOW_TITLE));
    resize(k_DIALOG_WIDTH, k_DIALOG_HEIGHT);

    const QPixmap baseLogo(k_LOGO_PATH);
    if (!baseLogo.isNull()) {
        QPixmap scaled = baseLogo.scaled(
            k_LOGO_SIZE, k_LOGO_SIZE,
            k_LOGO_ASPECT_RATIO_MODE,
            k_LOGO_TRANSFORM_MODE);
        scaled.setDevicePixelRatio(devicePixelRatioF());
        logoLabel->setPixmap(scaled);
    }
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    logoLabel->setStyleSheet(AboutDialogStyling::Styles::LOGO_LABEL_STYLE);

    textLabel->setText(buildAboutHtmlText());
    styleLabel(textLabel, true);
    textLabel->setStyleSheet(AboutDialogStyling::Styles::TEXT_LABEL_STYLE);
    textLabel->setOpenExternalLinks(true);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    layout->setSpacing(k_MAIN_SPACING);

    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
}
