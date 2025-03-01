#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include "titlebarmode.h"
#include "titlebarstyles.h"

#include <QPointer>
#include <QWidget>

// Forward declarations
class QLabel;
class QPushButton;

// CustomTitleBar class definition
class CustomTitleBar : public QWidget {
    Q_OBJECT

public:
    explicit CustomTitleBar(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    // Signals for window control
signals:
    void minimizeRequested();
    void closeRequested();

    // Internal setup methods
private:
    void setupLayout();
    QPushButton* createButton(const QString &label, const QString &style, const QString &tooltip);

    // Internal UI elements
private:
    QLabel *titleLabel;
    QPushButton *minimizeButton;
    QPushButton *closeButton;
};

// Factory function to create a custom title bar for a window
QPointer<CustomTitleBar> setupCustomTitleBar(QWidget *window, TitleBarMode mode);

#endif // CUSTOMTITLEBAR_H
