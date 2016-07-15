#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>

#include "Ui/databaseconnectionwidget.h"

#include "Image/imagemanager.h"

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
};

#endif // MAINWINDOW_H
