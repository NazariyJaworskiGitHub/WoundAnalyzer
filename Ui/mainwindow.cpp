#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _mousePosition = new QLabel("0,0",this);
    _managementMode = new QLabel("",this);
    _polygonArea = new QLabel("Wound area = 0 sq pix",this);
    _rulerDistance = new QLabel("Ruler lengt = 0 pix",this);

    ui->statusBar->addWidget(_mousePosition);
    ui->statusBar->addWidget(_managementMode);
    ui->statusBar->addWidget(_polygonArea);
    ui->statusBar->addWidget(_rulerDistance);

    // see ImageInterface::mouseMoveEvent()
    connect(ui->label,SIGNAL(mouseMoved_signal(QMouseEvent*)),
        this, SLOT(updateMousePositionStatus(QMouseEvent*)));

    // see ImageInterface::openImage()
    connect(ui->label,SIGNAL(updateFilename_signal(QString)),
        this, SLOT(updateImageFileName(QString)));

    // see ImageInterface::setManagementMode()
    connect(ui->label,SIGNAL(updateManagementMode_signal(ImageInterface::ManagementMode)),
        this, SLOT(updateManagementModeStatus(ImageInterface::ManagementMode)));

    // see ImageInterface::mousePressEvent()
    connect(ui->label,SIGNAL(updatePolygonArea_signal(int)),
        this, SLOT(updatePolygonArea(int)));

    // see ImageInterface::mousePressEvent()
    connect(ui->label,SIGNAL(updateRulerDistance_signal(double)),
        this, SLOT(updateRulerDistance(double)));

//    DatabaseConnectionWidget *_DBConnectionForm = new DatabaseConnectionWidget(this);
//    _DBConnectionForm->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMousePositionStatus(QMouseEvent *ev)
{
    _mousePosition->setText(QString::number(ev->x()) + "," + QString::number(ev->y()));
}

void MainWindow::updateManagementModeStatus(ImageInterface::ManagementMode mode)
{
    if(mode == ImageInterface::POLYGON_MODE)
        _managementMode->setText("Polygon mode");
    else if(mode == ImageInterface::RULER_MODE)
        _managementMode->setText("Ruler mode");
    else if(mode == ImageInterface::COMMON_MODE)
        _managementMode->setText("");
}

void MainWindow::updatePolygonArea(int area)
{
    _polygonArea->setText("Wound area = " + QString::number(area) + " sq pix");
}

void MainWindow::updateRulerDistance(double distance)
{
    _rulerDistance->setText("Wound area = " + QString::number(distance) + " pix");
}

void MainWindow::updateImageFileName(QString fileName)
{
    this->setWindowTitle("WoundAnalyzer: " + fileName);
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
    if(arg1)
    {
        ui->label->setManagementMode(ImageInterface::POLYGON_MODE);
        if(ui->actionRuler->isChecked())
            ui->actionRuler->setChecked(false);
    }
    else if(!ui->actionRuler->isChecked())
        ui->label->setManagementMode(ImageInterface::COMMON_MODE);
}
void MainWindow::on_actionRuler_toggled(bool arg1)
{
    if(arg1)
    {
        ui->label->setManagementMode(ImageInterface::RULER_MODE);
        if(ui->actionPolygon->isChecked())
            ui->actionPolygon->setChecked(false);
    }
    else if(!ui->actionPolygon->isChecked())
        ui->label->setManagementMode(ImageInterface::COMMON_MODE);
}

void MainWindow::on_actionClear_triggered()
{
    ui->label->clearStuff();
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

void MainWindow::on_actionSettings_triggered()
{
    SettingsWidget *_sw = new SettingsWidget(ui->label, this);
    _sw->exec();
}
