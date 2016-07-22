#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->imageHolderScrollArea);

    _mousePosition = new QLabel("0,0",this);
    _managementMode = new QLabel("Common mode, press and drag nodes",this);
    _polygonArea = new QLabel("Wound area = 0 sq pix",this);
    _rulerDistance = new QLabel("Ruler lengt = 0 pix",this);

    ui->statusBar->addWidget(_mousePosition);
    ui->statusBar->addWidget(_managementMode);
    ui->statusBar->addWidget(_polygonArea);
    ui->statusBar->addWidget(_rulerDistance);

    // see ImageInterface::mouseMoveEvent()
    connect(ui->image,SIGNAL(mouseMoved_signal(QMouseEvent*)),
        this, SLOT(updateMousePositionStatus(QMouseEvent*)));

    // see ImageInterface::openImage()
    connect(ui->image,SIGNAL(updateFilename_signal(QString)),
        this, SLOT(updateImageFileName(QString)));

    // see ImageInterface::setManagementMode()
    connect(ui->image,SIGNAL(updateManagementMode_signal(ImageInterface::ManagementMode)),
        this, SLOT(updateManagementModeStatus(ImageInterface::ManagementMode)));

    // see ImageInterface::mousePressEvent()
    connect(ui->image,SIGNAL(updatePolygonArea_signal(int)),
        this, SLOT(updatePolygonArea(int)));

    // see ImageInterface::mousePressEvent()
    connect(ui->image,SIGNAL(updateRulerDistance_signal(double)),
        this, SLOT(updateRulerDistance(double)));

    // see ImageInterface::mousePressEvent()
    connect(ui->image,SIGNAL(createNewNode_signal(QPoint)),
        this, SLOT(addNewNodeToList(QPoint)));
    connect(ui->image,SIGNAL(cleanPolygonNodes_signal()),
        this, SLOT(cleanNodeList()));
    connect(ui->image,SIGNAL(updateNode_signal(QPoint,QPoint)),
        this, SLOT(updateNode(QPoint,QPoint)));

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
        _managementMode->setText("Polygon mode, click to add node");
    else if(mode == ImageInterface::RULER_MODE)
        _managementMode->setText("Ruler mode, click to add node");
    else if(mode == ImageInterface::COMMON_MODE)
        _managementMode->setText("Common mode, press and drag nodes");
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

void MainWindow::addNewNodeToList(QPoint node)
{
    NodeSubItemWidget *w = new NodeSubItemWidget(node, this);
    ((QVBoxLayout*)ui->nodesScrollAreaWidgetContents->layout())->insertWidget(
                ui->nodesScrollAreaWidgetContents->layout()->count() - 1,
                w);
    connect(w,SIGNAL(sDelete(NodeSubItemWidget*)),
        this, SLOT(deleteNode(NodeSubItemWidget*)));
    _nodes.append(w);
}

void MainWindow::updateNode(QPoint node, QPoint newVal)
{
    for(QVector<NodeSubItemWidget*>::Iterator n = _nodes.begin(); n!= _nodes.end(); ++n)
        if((*n)->node.x() == node.x() && (*n)->node.y() == node.y())
        {
            (*n)->update(newVal);
            break;
        }
}

void MainWindow::cleanNodeList()
{
    for(QVector<NodeSubItemWidget*>::Iterator n = _nodes.begin(); n!= _nodes.end(); ++n)
    {
        ui->nodesScrollAreaWidgetContents->layout()->removeWidget(*n);
        delete *n;
    }
    _nodes.clear();
}

void MainWindow::deleteNode(NodeSubItemWidget *item)
{
    for(QVector<NodeSubItemWidget*>::Iterator n = _nodes.begin(); n!= _nodes.end(); ++n)
    {
        if((*n) == item)
        {
            ui->nodesScrollAreaWidgetContents->layout()->removeWidget(item);
            ui->image->deletePolygonNode(item->node);
            delete item;
            _nodes.erase(n);
            break;
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString _fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open wound image"),
                tr(""),
                tr("Image Files (*.bmp *.jpg *.jpeg *.png *.tif *.tiff)"));
    ui->image->openImage(_fileName);
}

void MainWindow::on_actionPolygon_toggled(bool arg1)
{
    if(arg1)
    {
        ui->image->setManagementMode(ImageInterface::POLYGON_MODE);
        if(ui->actionRuler->isChecked())
            ui->actionRuler->setChecked(false);
    }
    else if(!ui->actionRuler->isChecked())
        ui->image->setManagementMode(ImageInterface::COMMON_MODE);
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
        ui->image->setManagementMode(ImageInterface::COMMON_MODE);
}

void MainWindow::on_actionClear_triggered()
{
    ui->image->clearStuff();
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
    ImageManager::instance()->saveImage(_fileName,ui->image->transparency);
}
