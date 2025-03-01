#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QPointer>
#include <QDialog>
#include <QMouseEvent>

#include "../../ui/customtitlebar/customtitlebar.h"

// SettingsDialog class definition
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

    // Event handling for dragging
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // Internal slots
private slots:
    void onSaveClicked();
    void onCancelClicked();

    // Internal setup methods
private:
    void setupLayout();
    void setupConnections();

    // Internal UI elements
private:
    QPointer<CustomTitleBar> titleBar;

    // Internal state variables
private:
    QPoint lastMousePosition;
    bool dragging = false;
};

#endif // SETTINGSDIALOG_H
