#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QPointer>
#include <QWidget>

class QLabel;
class QPushButton;

class CustomTitleBar : public QWidget {
    Q_OBJECT

public:
    explicit CustomTitleBar(QWidget *parent = nullptr);

    // Provide size hints for layouts to work better
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

signals:
    void minimizeRequested();
    void closeRequested();

private:
    // Internal layout setup
    void setupLayout();

    // Helper to create buttons consistently
    QPushButton* createButton(const QString &label, const QString &style, const QString &tooltip);

    // Title bar components
    QPointer<QLabel> titleLabel;
    QPointer<QPushButton> minimizeButton;
    QPointer<QPushButton> closeButton;
};

#endif // CUSTOMTITLEBAR_H
