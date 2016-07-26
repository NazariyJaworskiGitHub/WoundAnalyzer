#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QMessageBox>

#include "Ui/imageinterface.h"
#include "Ui/settingswidget.h"
#include "Ui/databaseconnectionwidget.h"

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
    QLabel *_polygonArea = nullptr;
    QLabel *_rulerDistance = nullptr;
    Ui::MainWindow *ui;

private Q_SLOTS:
    void updateMousePositionStatus(QMouseEvent *ev);
    void updateManagementModeStatus(ImageInterface::ManagementMode mode);
    void updatePolygonArea(double area);
    void updateRulerDistance(double distance);
    void updateImageFileNameAndResetZoom(QString fileName);
    void blockInterface(bool b);
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
};

#endif // MAINWINDOW_H
