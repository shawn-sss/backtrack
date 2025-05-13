// Project includes
#include "aboutdialog.h"
#include "AboutDialogStyling.h"
#include "../../../../constants/app_info.h"

// Qt includes
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPixmap>
#include <QSizePolicy>

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("About");
    resize(500, 400);

    logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/resources/icons/app_icon.png");
    logoLabel->setPixmap(logoPixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    textLabel = new QLabel(this);
    textLabel->setText(QString(R"(
        <h2>%1</h2>
        <p>Version: <b>%2</b></p>
        <p>Author: <b>%3</b></p>
        <p style='margin-top:15px;'>A simple, efficient backup utility focused on transparency, control, and clarity.</p>
    )").arg(App::Info::k_NAME,
                                App::Info::k_VERSION,
                                App::Info::k_AUTHOR_NAME));
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setWordWrap(true);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    AboutDialogStyling::applyCursors(buttonBox);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(logoLabel);
    layout->addWidget(textLabel);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

AboutDialog::~AboutDialog() = default;
