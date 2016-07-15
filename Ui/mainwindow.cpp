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
    ImageManager::instance()->openImage(_fileName);
    ui->label->setPixmap(ImageManager::instance()->getImageAsQPixmap());
}
