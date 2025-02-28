#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QWidget>
#include <QPointer>
#include "titlebarmode.h"  // Add this

class QLabel;
class QPushButton;

class CustomTitleBar : public QWidget {
    Q_OBJECT

public:
    explicit CustomTitleBar(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

signals:
    void minimizeRequested();
    void closeRequested();

private:
    QLabel *titleLabel;
    QPushButton *minimizeButton;
    QPushButton *closeButton;

    void setupLayout();
    QPushButton* createButton(const QString &label, const QString &style, const QString &tooltip);
};

QPointer<CustomTitleBar> setupCustomTitleBar(QWidget *window, TitleBarMode mode);  // stays here

#endif // CUSTOMTITLEBAR_H
