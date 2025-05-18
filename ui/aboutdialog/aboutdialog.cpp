// Project includes
#include "aboutdialog.h"
#include "aboutdialogconstants.h"
#include "AboutDialogStyling.h"
#include "../../../../constants/app_info.h"

// Qt includes
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>

// Constructs and sets up the About dialog
AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("About");
    resize(AboutDialogConstants::kDialogWidth, AboutDialogConstants::kDialogHeight);

    logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/resources/icons/app_icon.png");
    logoLabel->setPixmap(logoPixmap.scaled(AboutDialogConstants::kLogoSize, AboutDialogConstants::kLogoSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    textLabel = new QLabel(this);
    const QString htmlText =
        QString("<h2>%1</h2>"
                "<p>Version: <b>%2</b></p>"
                "<p>Author: <b>%3</b></p>"
                "<p style='margin-top:15px;'>A simple, efficient backup utility focused on transparency, control, and clarity.</p>")
            .arg(App::Info::k_APP_NAME,
                 App::Info::k_APP_VERSION,
                 App::Info::k_AUTHOR_NAME);
    textLabel->setText(htmlText);
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setWordWrap(true);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    AboutDialogStyling::applyCursors(buttonBox);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
    layout->addWidget(buttonBox);
    setLayout(layout);
}
