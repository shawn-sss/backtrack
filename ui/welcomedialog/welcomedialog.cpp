// Project includes
#include "welcomedialog.h"
#include "welcomedialogconstants.h"
#include "welcomedialogstyling.h"
#include "../../constants/app_info.h"

// Qt includes
#include <QLabel>
#include <QPixmap>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace {

QString buildWelcomeHtmlText() {
    using namespace WelcomeDialogConstants;
    return QString(k_WELCOME_HTML)
        .arg(App::Info::k_APP_NAME,
             App::Info::k_APP_VERSION,
             QString::number(k_TOP_MARGIN));
}

void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setTextFormat(richText ? Qt::RichText : Qt::PlainText);
}

} // namespace

// Construct welcome dialog window
WelcomeDialog::WelcomeDialog(QWidget* parent)
    : QDialog(parent),
    logoLabel(new QLabel(this)),
    textLabel(new QLabel(this))
{
    using namespace WelcomeDialogConstants;
    using namespace WelcomeDialogStyling::Styles;

    setWindowTitle(tr(k_WINDOW_TITLE));
    resize(k_DIALOG_WIDTH, k_DIALOG_HEIGHT);

    const QPixmap baseLogo(QString::fromLatin1(k_LOGO_RESOURCE_PATH));
    if (!baseLogo.isNull()) {
        QPixmap scaled = baseLogo.scaled(
            k_LOGO_SIZE,
            k_LOGO_SIZE,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
        scaled.setDevicePixelRatio(devicePixelRatioF());
        logoLabel->setPixmap(scaled);
    }
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    logoLabel->setStyleSheet(LOGO_LABEL_STYLE);

    textLabel->setText(buildWelcomeHtmlText());
    styleLabel(textLabel, true);
    textLabel->setStyleSheet(TEXT_LABEL_STYLE);
    textLabel->setOpenExternalLinks(true);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN, k_MAIN_MARGIN);
    layout->setSpacing(k_MAIN_SPACING);

    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
    layout->addStretch();
}
