// Project includes
#include "welcomedialog.h"
#include "welcomedialogconstants.h"
#include "welcomedialogstyling.h"
#include "../../constants/app_info.h"

// Qt includes
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPixmap>
#include <QSizePolicy>
#include <QVBoxLayout>

using namespace WelcomeDialogConstants;

// Applies pointing hand cursor to dialog buttons
static void applyCursors(QDialogButtonBox* buttonBox) {
    const QList<QAbstractButton*>& buttons = buttonBox->buttons();
    for (auto* button : buttons)
        button->setCursor(Qt::PointingHandCursor);
}

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

    const QPixmap logoPixmap(k_LOGO_RESOURCE_PATH);
    logoLabel->setPixmap(logoPixmap.scaled(
        kLogoSize,
        kLogoSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    logoLabel->setStyleSheet(WelcomeDialogStyling::Styles::LOGO_LABEL_STYLE);

    textLabel->setText(buildWelcomeHtmlText());
    styleLabel(textLabel, true);
    textLabel->setStyleSheet(WelcomeDialogStyling::Styles::TEXT_LABEL_STYLE);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    applyCursors(buttonBox);
    buttonBox->setStyleSheet(WelcomeDialogStyling::Styles::BUTTON_STYLE);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

// Destructor
WelcomeDialog::~WelcomeDialog() = default;
