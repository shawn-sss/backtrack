// filename: welcomedialog.cpp

// Project includes
#include "welcomedialog.h"
#include "welcomedialogconstants.h"
#include "welcomedialogstyling.h"
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

QString buildWelcomeHtmlText() {
    using namespace WelcomeDialogConstants;
    return k_WELCOME_HTML
        .arg(App::Info::k_APP_NAME,
             App::Info::k_APP_VERSION,
             QString::number(kTopMargin));
}

void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setTextFormat(richText ? Qt::RichText : Qt::PlainText);
}

} // namespace

// Constructs WelcomeDialog and sets up UI
WelcomeDialog::WelcomeDialog(QWidget* parent)
    : QDialog(parent),
    logoLabel(new QLabel(this)),
    textLabel(new QLabel(this)),
    buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok, this)) {

    setWindowTitle(tr(WelcomeDialogConstants::k_WINDOW_TITLE));
    resize(WelcomeDialogConstants::kDialogWidth, WelcomeDialogConstants::kDialogHeight);

    const QPixmap baseLogo(QString::fromLatin1(WelcomeDialogConstants::k_LOGO_RESOURCE_PATH));
    if (!baseLogo.isNull()) {
        QPixmap scaled = baseLogo.scaled(
            WelcomeDialogConstants::kLogoSize,
            WelcomeDialogConstants::kLogoSize,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
        scaled.setDevicePixelRatio(devicePixelRatioF());
        logoLabel->setPixmap(scaled);
    }
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    logoLabel->setStyleSheet(WelcomeDialogStyling::Styles::LOGO_LABEL_STYLE);

    textLabel->setText(buildWelcomeHtmlText());
    styleLabel(textLabel, true);
    textLabel->setStyleSheet(WelcomeDialogStyling::Styles::TEXT_LABEL_STYLE);
    textLabel->setOpenExternalLinks(true);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    for (auto* button : buttonBox->buttons()) {
        if (auto* pushButton = qobject_cast<QPushButton*>(button)) {
            Shared::UI::applyButtonTooltipAndCursor(pushButton, tr(WelcomeDialogConstants::k_OK_BUTTON_TOOLTIP));
        }
    }

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
    layout->addWidget(buttonBox);
}
