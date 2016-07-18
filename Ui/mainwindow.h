#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QMessageBox>

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
    Ui::MainWindow *ui;

private Q_SLOTS:
    void on_actionOpen_triggered();
    void on_actionPolygon_toggled(bool arg1);
    void on_actionClear_Polygon_triggered();
    void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H
