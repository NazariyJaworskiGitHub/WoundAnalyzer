#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QMessageBox>

#include "Ui/imageinterface.h"
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
    void updatePolygonArea(int area);
    void updateRulerDistance(double distance);
    void updateImageFileName(QString fileName);
    void on_actionOpen_triggered();
    void on_actionPolygon_toggled(bool arg1);
    void on_actionClear_triggered();
    void on_actionAbout_triggered();
    void on_actionRuler_toggled(bool arg1);
};

#endif // MAINWINDOW_H
