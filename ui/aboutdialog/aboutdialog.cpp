// Project includes
#include "aboutdialog.h"
#include "aboutdialogconstants.h"
#include "aboutdialogstyling.h"
#include "../../constants/app_info.h"

// Qt includes
#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QAbstractButton>
#include <QPixmap>
#include <QSizePolicy>

// Applies pointing hand cursor to dialog buttons
static void applyCursors(QDialogButtonBox* buttonBox) {
    const QList<QAbstractButton*>& buttons = buttonBox->buttons();
    for (int i = 0; i < buttons.size(); ++i)
        buttons[i]->setCursor(Qt::PointingHandCursor);
}

// Builds the HTML content for the About dialog text
static QString buildAboutHtmlText() {
    return QStringLiteral(
               "<h2>%1</h2>"
               "<p>Version: <b>%2</b></p>"
               "<p>Author: <b>%3</b></p>"
               "<p style='margin-top:%4px;'>%5</p>")
        .arg(App::Info::k_APP_NAME,
             App::Info::k_APP_VERSION,
             App::Info::k_AUTHOR_NAME,
             QString::number(AboutDialogConstants::kTopMargin),
             App::Info::k_APP_ABOUT);
}

// Styles a QLabel for centered, wrapped rich/plain text
static void styleLabel(QLabel* label, bool richText = false) {
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setTextFormat(richText ? Qt::RichText : Qt::PlainText);
}

// Constructs the About dialog
AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent),
    logoLabel(new QLabel(this)),
    textLabel(new QLabel(this)),
    buttonBox(new QDialogButtonBox(QDialogButtonBox::Close, this)) {

    setWindowTitle(tr("About"));
    resize(AboutDialogConstants::kDialogWidth, AboutDialogConstants::kDialogHeight);

    const QPixmap logoPixmap(":/resources/icons/app_icon.png");
    logoLabel->setPixmap(logoPixmap.scaled(
        AboutDialogConstants::kLogoSize,
        AboutDialogConstants::kLogoSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    logoLabel->setStyleSheet(AboutDialogStyling::Styles::LOGO_LABEL_STYLE);

    textLabel->setText(buildAboutHtmlText());
    styleLabel(textLabel, true);
    textLabel->setStyleSheet(AboutDialogStyling::Styles::TEXT_LABEL_STYLE);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    applyCursors(buttonBox);
    buttonBox->setStyleSheet(AboutDialogStyling::Styles::BUTTON_STYLE);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
    layout->addWidget(buttonBox);
    setLayout(layout);
}
