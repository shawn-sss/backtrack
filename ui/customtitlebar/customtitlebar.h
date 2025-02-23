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

signals:
    void minimizeRequested();
    void closeRequested();

private:
    // Layout setup
    void setupLayout();

    // Button creation
    QPushButton* createButton(const QString &label, const QString &style, const QString &tooltip);

    QPointer<QLabel> titleLabel;
    QPointer<QPushButton> minimizeButton;
    QPointer<QPushButton> closeButton;
};

#endif
