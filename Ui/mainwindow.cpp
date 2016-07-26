#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->centralWidget);

    ui->imageZoomFactorLabel->setText(
                QString::number(ImageManager::instance()->zoomFactor * 100) + "%");
    ui->imageTransparencyLabel->setText(
                QString::number(ImageManager::instance()->drawingLayerTransparency*100) + "%");

    _mousePosition = new QLabel("0,0",this);
    _managementMode = new QLabel("Edit mode, LMB to drag nodes, RMB to delete polygon nodes or split edges",this);
    _polygonArea = new QLabel("Total wound area = 0",this);
    _rulerDistance = new QLabel("Ruler lengt = 0px",this);

    ui->statusBar->addWidget(_mousePosition);
    ui->statusBar->addWidget(_managementMode);
    ui->statusBar->addWidget(_polygonArea);
    ui->statusBar->addWidget(_rulerDistance);

    connect(ui->image,SIGNAL(mouseMoved_signal(QMouseEvent*)),
        this, SLOT(updateMousePositionStatus(QMouseEvent*)));

    connect(ui->image,SIGNAL(updateFilenameAndResetZoom_signal(QString)),
        this, SLOT(updateImageFileNameAndResetZoom(QString)));

    connect(ui->image,SIGNAL(updateManagementMode_signal(ImageInterface::ManagementMode)),
        this, SLOT(updateManagementModeStatus(ImageInterface::ManagementMode)));

    connect(ui->image,SIGNAL(updatePolygonArea_signal(double)),
        this, SLOT(updatePolygonArea(double)));

    connect(ui->image,SIGNAL(updateRulerDistance_signal(double)),
        this, SLOT(updateRulerDistance(double)));

    blockInterface(true);

    ui->actionConnect->setDisabled(true);
    ui->actionRecord->setDisabled(true);
    ui->actionContext->setDisabled(true);
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
        _managementMode->setText("Polygon mode, click to add new polygon nodes");
    else if(mode == ImageInterface::RULER_MODE)
        _managementMode->setText("Ruler mode, click to add node");
    else if(mode == ImageInterface::EDIT_MODE)
        _managementMode->setText("Edit mode, LMB to drag nodes, RMB to delete polygon nodes or split edges");
}

void MainWindow::updatePolygonArea(double area)
{
    _polygonArea->setText("Total wound area = " + QString::number(area));
}

void MainWindow::updateRulerDistance(double distance)
{
    _rulerDistance->setText("Ruler distance = " + QString::number(distance) + "px");
}

void MainWindow::updateImageFileNameAndResetZoom(QString fileName)
{
    this->setWindowTitle("WoundAnalyzer: " + fileName);
    ui->imageZoomFactorLabel->setText(
                QString::number(ImageManager::instance()->zoomFactor * 100) + "%");
    ui->zoomSlider->blockSignals(true);
    ui->zoomSlider->setValue(ImageManager::instance()->zoomFactor * 100);
    ui->zoomSlider->blockSignals(false);
}

void MainWindow::blockInterface(bool b)
{
    ui->actionPolygon->setDisabled(b);
    ui->actionRuler->setDisabled(b);
    ui->actionClear->setDisabled(b);
    ui->actionSave->setDisabled(b);
    ui->actionSettings->setDisabled(b);
    ui->actionZoom_In->setDisabled(b);
    ui->actionZoom_Out->setDisabled(b);
    ui->zoomSlider->setDisabled(b);
    ui->imageZoomFactorLabel->setDisabled(b);
    ui->imageZoomFactorIcon->setDisabled(b);
    ui->rulerFactorDoubleSpinBox->setDisabled(b);
    ui->RulerFactorLabel->setDisabled(b);
    ui->transparencySlider->setDisabled(b);
    ui->imageTransparencyLabel->setDisabled(b);
}

void MainWindow::on_actionOpen_triggered()
{
    QString _fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open wound image"),
                tr(""),
                tr("Image Files (*.bmp *.jpg *.jpeg *.png *.tif *.tiff)"));
    if(_fileName.size())
    {
        ui->image->openImage(_fileName);
        blockInterface(false);
    }
}

void MainWindow::on_actionPolygon_toggled(bool arg1)
{
    if(arg1)
    {
        ui->image->createNewPolygon = true;
        ui->image->setManagementMode(ImageInterface::POLYGON_MODE);
        if(ui->actionRuler->isChecked())
            ui->actionRuler->setChecked(false);
    }
    else if(!ui->actionRuler->isChecked())
        ui->image->setManagementMode(ImageInterface::EDIT_MODE);
}
void MainWindow::on_actionRuler_toggled(bool arg1)
{
    if(arg1)
    {
        ui->image->setManagementMode(ImageInterface::RULER_MODE);
        if(ui->actionPolygon->isChecked())
            ui->actionPolygon->setChecked(false);
    }
    else if(!ui->actionPolygon->isChecked())
        ui->image->setManagementMode(ImageInterface::EDIT_MODE);
}

void MainWindow::on_actionClear_triggered()
{
    ui->image->clearAll();
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
    SettingsWidget *_sw = new SettingsWidget(ui->image, this);
    _sw->exec();
}

void MainWindow::on_actionSave_triggered()
{
    QString _fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save wound image"),
                tr(""),
                tr("Image Files (*.bmp *.jpg *.jpeg *.png *.tif *.tiff)"));
    ImageManager::instance()->saveImage(_fileName);
}

void MainWindow::on_actionZoom_In_triggered()
{
    if(ImageManager::instance()->zoomFactor < 3.9999)
    {
        ui->image->zoom(0.1);
        ui->imageZoomFactorLabel->setText(
                    QString::number(ImageManager::instance()->zoomFactor * 100) + "%");
        ui->zoomSlider->blockSignals(true);
        ui->zoomSlider->setValue(ImageManager::instance()->zoomFactor * 100);
        ui->zoomSlider->blockSignals(false);
    }
}

void MainWindow::on_actionZoom_Out_triggered()
{
    if(ImageManager::instance()->zoomFactor > 0.5001)
    {
        ui->image->zoom(-0.1);
        ui->imageZoomFactorLabel->setText(
                    QString::number(ImageManager::instance()->zoomFactor * 100) + "%");
        ui->zoomSlider->blockSignals(true);
        ui->zoomSlider->setValue(ImageManager::instance()->zoomFactor * 100);
        ui->zoomSlider->blockSignals(false);
    }
}

void MainWindow::on_zoomSlider_valueChanged(int value)
{
    double delta = value/100.0 - ImageManager::instance()->zoomFactor;
    ui->image->zoom(delta);
    ui->imageZoomFactorLabel->setText(
                QString::number(ImageManager::instance()->zoomFactor * 100) + "%");
}

void MainWindow::on_rulerFactorDoubleSpinBox_valueChanged(double arg1)
{
    ImageManager::instance()->rulerFactor = arg1;
    ui->image->drawAll();
}

void MainWindow::on_transparencySlider_valueChanged(int value)
{
    ImageManager::instance()->drawingLayerTransparency = value/100.0;
    ui->imageTransparencyLabel->setText(QString::number(value) + "%");
    ui->image->drawAll();
}
