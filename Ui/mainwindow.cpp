#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DatabaseConnectionWidget *_DBConnectionForm = new DatabaseConnectionWidget(this);

    _DBConnectionForm->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
