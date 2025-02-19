#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QWidget>
#include <QPointer>

class QLabel;
class QPushButton;

class CustomTitleBar : public QWidget {
    Q_OBJECT

public:
    explicit CustomTitleBar(QWidget *parent = nullptr);

signals:
    void minimizeRequested();
    void closeRequested();

private:
    // Sets up the title bar layout
    void setupLayout();

    // Creates a styled button
    QPushButton* createButton(const QString &label, const QString &style, const QString &tooltip);

    QPointer<QLabel> titleLabel;
    QPointer<QPushButton> minimizeButton;
    QPointer<QPushButton> closeButton;
};

#endif // CUSTOMTITLEBAR_H
