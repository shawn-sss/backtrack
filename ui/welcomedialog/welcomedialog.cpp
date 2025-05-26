// Project includes
#include "welcomedialog.h"
#include "welcomedialogconstants.h"
#include "welcomedialogstyling.h"
#include "../../constants/app_info.h"

// Qt includes
#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPixmap>
#include <QSizePolicy>
#include <QAbstractButton>

// Applies pointing hand cursor to dialog buttons
static void applyCursors(QDialogButtonBox* buttonBox) {
    const QList<QAbstractButton*>& buttons = buttonBox->buttons();
    for (auto* button : buttons)
        button->setCursor(Qt::PointingHandCursor);
}

// Constructs the welcome message HTML
static QString buildWelcomeHtmlText() {
    return QStringLiteral(
               "<h2>Welcome to %1</h2>"
               "<p>Version: <b>%2</b></p>"
               "<p>Thank you for installing our application!</p>"
               "<p style='margin-top:%3px;'>Let's get you started...</p>")
        .arg(App::Info::k_APP_NAME,
             App::Info::k_APP_VERSION,
             QString::number(WelcomeDialogConstants::kTopMargin));
}

// Styles a QLabel for centered, wrapped rich text
static void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setTextFormat(richText ? Qt::RichText : Qt::PlainText);
}

// Constructs the WelcomeDialog
WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent),
    logoLabel(new QLabel(this)),
    textLabel(new QLabel(this)),
    buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok, this)) {

    setWindowTitle(tr("Welcome"));
    resize(WelcomeDialogConstants::kDialogWidth, WelcomeDialogConstants::kDialogHeight);

    const QPixmap logoPixmap(":/resources/icons/app_icon.png");
    logoLabel->setPixmap(logoPixmap.scaled(
        WelcomeDialogConstants::kLogoSize,
        WelcomeDialogConstants::kLogoSize,
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
