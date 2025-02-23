#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "../customtitlebar/customtitlebar.h"

#include <memory>
#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include <QPointer>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

protected:
    // Mouse event handlers
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // UI setup
    void setupUIComponents();
    void setupConnections();

private slots:
    // Button actions
    void onSaveClicked();
    void onCancelClicked();

private:
    std::unique_ptr<Ui::SettingsDialog> ui;
    QPointer<CustomTitleBar> titleBar;
    QPoint lastMousePosition;
    bool dragging{false};
};

#endif // SETTINGSDIALOG_H
