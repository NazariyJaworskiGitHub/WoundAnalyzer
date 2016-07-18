#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    DatabaseConnectionWidget *_DBConnectionForm = new DatabaseConnectionWidget(this);
//    _DBConnectionForm->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString _fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open wound image"),
                tr(""),
                tr("Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.tif *.tiff *.tga)"));
    ui->label->openImage(_fileName);
}

void MainWindow::on_actionPolygon_toggled(bool arg1)
{
    ui->actionPolygon->setChecked(arg1);
    ui->label->setPolygonMode(arg1);
}

void MainWindow::on_actionClear_Polygon_triggered()
{
    ui->label->cleanPolygon();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("<b>Wound Analyzer</b>");
    msgBox.setInformativeText("pre Aplha by Nazariy Jaworski 2016 (c)");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
