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
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QPushButton>

using namespace WelcomeDialogConstants;

// Constructs the welcome message HTML
static QString buildWelcomeHtmlText() {
    return QString(k_WELCOME_HTML)
    .arg(App::Info::k_APP_NAME,
         App::Info::k_APP_VERSION,
         QString::number(kTopMargin));
}

// Styles a QLabel for centered, wrapped rich text
static void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setTextFormat(richText ? Qt::RichText : Qt::PlainText);
}

// Constructs the welcome dialog with styled logo, message, and OK button
WelcomeDialog::WelcomeDialog(QWidget* parent)
    : QDialog(parent),
    logoLabel(new QLabel(this)),
    textLabel(new QLabel(this)),
    buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok, this)) {

    setWindowTitle(tr(k_WINDOW_TITLE));
    resize(kDialogWidth, kDialogHeight);

    // Logo setup
    const QPixmap logoPixmap(k_LOGO_RESOURCE_PATH);
    logoLabel->setPixmap(logoPixmap.scaled(
        kLogoSize,
        kLogoSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    logoLabel->setStyleSheet(WelcomeDialogStyling::Styles::LOGO_LABEL_STYLE);

    // Text setup
    textLabel->setText(buildWelcomeHtmlText());
    styleLabel(textLabel, true);
    textLabel->setStyleSheet(WelcomeDialogStyling::Styles::TEXT_LABEL_STYLE);

    // Connect button
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    // ✅ Centralized button handling with safe cast
    for (QAbstractButton* button : buttonBox->buttons()) {
        if (auto* pushButton = qobject_cast<QPushButton*>(button)) {
            Shared::UI::applyButtonTooltipAndCursor(pushButton, tr("OK"));
        }
    }

    // Layout setup
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

// Destructor
WelcomeDialog::~WelcomeDialog() = default;
