#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Project includes same directory
#include "../customtitlebar/customtitlebar.h"
#include "../customtitlebar/customtitlebarmode.h"

// Built-in Qt includes
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMouseEvent>

// Dialog window for application settings with a custom title bar
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    // Constructor and Destructor
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

protected:
    // Mouse event handlers for window dragging
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Initializes UI layout
    void setupLayout();
    // Establishes signal connections
    void setupConnections();

    // Custom title bar component
    CustomTitleBar *titleBar;
    // Dragging state
    bool dragging;
    // Last recorded mouse position for dragging
    QPoint lastMousePosition;

private slots:
    // Handles save action
    void onSaveClicked();
    // Handles cancel action
    void onCancelClicked();
};

#endif // SETTINGSDIALOG_H
