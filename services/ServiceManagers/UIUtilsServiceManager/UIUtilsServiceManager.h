#ifndef UIUTILSSERVICEMANAGER_H
#define UIUTILSSERVICEMANAGER_H

// Qt includes
#include <QMouseEvent>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QTabWidget>
#include <QTreeView>
#include <QWidget>
#include <QString>
#include <QList>
#include <QPair>
#include <QPoint>

namespace Ui { class MainWindow; }

namespace Shared::UI {

// Window dragging
void handleMousePress(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseMove(QWidget* window, QMouseEvent* event, bool& dragging, QPoint& lastMousePosition);
void handleMouseRelease(QMouseEvent* event, bool& dragging);

// Tree view
void removeAllColumnsFromTreeView(QTreeView* treeView, int startColumn, int columnCount);

// Progress bar
void setupProgressBar(QProgressBar* progressBar, int minValue, int maxValue, int height, bool textVisible);

// Tab widget
void setTabWidgetCursorToPointer(QTabWidget* tabWidget);

// Status light pixmap
QPixmap createStatusLightPixmap(const QString& color, int size);

// Button styling
void applyButtonTooltipAndCursor(QPushButton* button, const QString& tooltip);
void applyButtonStyling(const QList<QPair<QPushButton*, QString>>& buttonTooltipPairs);
void applyButtonStylingWithObjectName(QPushButton* button, const QString& tooltip, const QString& objectName);

// Centralized styling manager
class UIUtilsServiceManager {
public:
    static QList<QPair<QPushButton*, QString>> mainWindowButtonMappings(Ui::MainWindow* ui);
    static void applyMainWindowButtonStyling(Ui::MainWindow* ui);
    static void applyTemplateResetButtonStyling(QPushButton* button);
};

} // namespace Shared::UI

#endif
