#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QMessageBox>

#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QTextCursor>
#include <QDesktopServices>
#include <QDate>
#include <QToolBar>
#include <QDockWidget>

#include "Ui/imageinterface.h"
#include "Ui/settingswidget.h"
#include "Ui/databaseconnectionwidget.h"

#include "Utilities/Logger/logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QLabel *_mousePosition = nullptr;
    QLabel *_managementMode = nullptr;
    QLabel *_rulerDistance = nullptr;
    QToolBar *_DBToolbar = nullptr;
    Ui::MainWindow *ui;

    QString _imageFileName;

private Q_SLOTS:
    void updateMousePositionStatus(QMouseEvent *ev);
    void updateManagementModeStatus(ImageInterface::ManagementMode mode);
    void updatePolygonArea(double area);
    void updateRulerDistance(double distance);
    void updateImageFileNameAndResetZoom(QString fileName);
    void blockMainActions(bool b);
    void blockDatabaseActions(bool b);
    void on_actionOpen_triggered();
    void on_actionPolygon_toggled(bool arg1);
    void on_actionClear_triggered();
    void on_actionAbout_triggered();
    void on_actionRuler_toggled(bool arg1);
    void on_actionSettings_triggered();
    void on_actionSave_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_zoomSlider_valueChanged(int value);
    void on_rulerFactorDoubleSpinBox_valueChanged(double arg1);
    void on_transparencySlider_valueChanged(int value);
    void on_actionRecord_triggered();
    void on_actionExport_results_triggered();
    void on_actionContext_triggered();
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_doubleSpinBox_2_valueChanged(double arg1);
    void on_spinBox_valueChanged(int arg1);
    void on_doubleSpinBox_3_valueChanged(double arg1);
    void on_actionConnect_triggered();
    void on_treeView_activated(const QModelIndex &index);
    void on_treeView_clicked(const QModelIndex &index);
    void on_actionUpdate_triggered();
};

#endif // MAINWINDOW_H
