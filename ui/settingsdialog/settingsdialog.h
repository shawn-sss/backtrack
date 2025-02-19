#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include <memory>
#include <QPointer>

#include "../customtitlebar/customtitlebar.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

protected:
    // Handles mouse press events
    void mousePressEvent(QMouseEvent *event) override;

    // Handles mouse move events
    void mouseMoveEvent(QMouseEvent *event) override;

    // Handles mouse release events
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Sets up UI components
    void setupUIComponents();

    // Sets up signal connections
    void setupConnections();

private slots:
    // Handles save action
    void onSaveClicked();

    // Handles cancel action
    void onCancelClicked();

private:
    std::unique_ptr<Ui::SettingsDialog> ui;
    QPointer<CustomTitleBar> titleBar;
    QPoint lastMousePosition;
    bool dragging{false};
};

#endif // SETTINGSDIALOG_H
