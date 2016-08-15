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
    _rulerDistance = new QLabel("Ruler lengt = 0px",this);

    ui->statusBar->addWidget(_mousePosition);
    ui->statusBar->addWidget(_managementMode);
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

    _DBToolbar = new QToolBar(this);
    _DBToolbar->addAction(ui->actionAdd);
    _DBToolbar->addAction(ui->actionDelete);
    _DBToolbar->addAction(ui->actionUpdate);
    _DBToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->databaseHolderContents->layout()->addWidget(_DBToolbar);

    blockMainActions(true);
    blockDatabaseActions(true);

    showMaximized();
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
    ui->lineEditArea->setText(QString::number(area));
}

void MainWindow::updateRulerDistance(double distance)
{
    _rulerDistance->setText("Ruler distance = " + QString::number(distance) + "px");
}

void MainWindow::updateImageFileNameAndResetZoom(QString fileName)
{
    _imageFileName = fileName;
    this->setWindowTitle("WoundAnalyzer: [" + fileName +"]");
    ui->imageZoomFactorLabel->setText(
                QString::number(ImageManager::instance()->zoomFactor * 100) + "%");
    ui->zoomSlider->blockSignals(true);
    ui->zoomSlider->setValue(ImageManager::instance()->zoomFactor * 100);
    ui->zoomSlider->blockSignals(false);
}

void MainWindow::blockMainActions(bool b)
{
    ui->actionPolygon->setDisabled(b);
    ui->actionRuler->setDisabled(b);
    ui->actionClear->setDisabled(b);
    ui->actionSave->setDisabled(b);
    ui->actionSettings->setDisabled(b);
    ui->actionZoom_In->setDisabled(b);
    ui->actionZoom_Out->setDisabled(b);
    ui->actionExport_results->setDisabled(b);
    ui->zoomSlider->setDisabled(b);
    ui->imageZoomFactorLabel->setDisabled(b);
    ui->imageZoomFactorIcon->setDisabled(b);
    ui->rulerFactorDoubleSpinBox->setDisabled(b);
    ui->RulerFactorLabel->setDisabled(b);
    ui->transparencySlider->setDisabled(b);
    ui->imageTransparencyLabel->setDisabled(b);
    ui->labelArea->setDisabled(b);
    ui->labelMM2->setDisabled(b);
    ui->lineEditArea->setDisabled(b);
}

void MainWindow::blockDatabaseActions(bool b)
{
    ui->actionAdd->setDisabled(b);
    ui->actionDelete->setDisabled(b);
    ui->actionUpdate->setDisabled(b);
    ui->databaseNameLineEdit->setDisabled(b);
    ui->databaseNotesPlainTextEdit->setDisabled(b);
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
        blockMainActions(false);
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
                tr("JPG (*.jpg);;PNG (*.png);;BMP (*.bmp);;TIF (*.tif)"));
    if(_fileName.size())
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

void MainWindow::on_actionRecord_triggered()
{
    /// \todo
}

void MainWindow::on_actionExport_results_triggered()
{    
    QString _fileName = QFileDialog::getSaveFileName(
                0,
                QObject::tr("Save result"),
                QObject::tr(""),
                QObject::tr("PDF Files (*.pdf)"));
    if(_fileName.size())
    {
        Log::StaticLogger::instance() << "[Utilities] exporting results to pdf file\n";

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(_fileName);
        QTextDocument doc;
        QTextCursor cursor(&doc);
        //cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        cursor.insertHtml(
                    "<h1><center>Wound Analysis Report [" +
                    QString::number(QDate::currentDate().month()) + "." +
                    QString::number(QDate::currentDate().day()) + "." +
                    QString::number(QDate::currentDate().year()) + "]</<center></h1>" +
                    "<p><b>Image: </b> " + _imageFileName + "</p><br>");
        cursor.insertImage(ImageManager::instance()->getImageAsQImage());
        cursor.insertHtml(
                    "<br><p><b>Total wound area:</b> " + ui->lineEditArea->text() + " sm<sup>2</sup>" +
                    "</p>");
        // This is necessary if you want to hide the page number
        //doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);

        bool _result = QDesktopServices::openUrl(QUrl::fromLocalFile(_fileName));
        if(_result)
            Log::StaticLogger::instance() << "[Utilities] results are shown\n";
        else
            Log::StaticLogger::instance() << "[Utilities] <FAIL> results are not shown\n";
    }
}

void MainWindow::on_actionContext_triggered()
{
    Log::StaticLogger::instance() << "[Utilities] openint context file\n";
    bool _result = QDesktopServices::openUrl(QUrl::fromLocalFile("Wound analyzer context.pdf"));
    if(_result)
        Log::StaticLogger::instance() << "[Utilities] context file is opened\n";
    else
        Log::StaticLogger::instance() << "[Utilities] <FAIL> context file is not opened\n";
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    ImageManager::instance()->filterFactorA = arg1;
    ImageManager::instance()->applyFiltration();
    ui->image->drawAll();
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    ImageManager::instance()->filterFactorB = arg1;
    ImageManager::instance()->applyFiltration();
    ui->image->drawAll();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ImageManager::instance()->filterIterations = arg1;
    ImageManager::instance()->applyFiltration();
    ui->image->drawAll();
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    ImageManager::instance()->filterFactorC = arg1;
    ImageManager::instance()->applyFiltration();
    ui->image->drawAll();
}

void MainWindow::on_actionConnect_triggered()
{
    DatabaseConnectionWidget *_DBConnectionForm = new DatabaseConnectionWidget(this);
    _DBConnectionForm->exec();

    if(DatabaseManager::instance()->isConnected())
    {
        ui->treeView->setModel(DatabaseManager::instance()->prepareDatabaseModel(this));
        ui->treeView->show();
    }
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(index);
    switch (item->type()) {
    case SURVEY_TYPE:
    {
        Survey *survey = static_cast<Survey*>(item);
        if(survey->image.empty())
        {
            if(DatabaseManager::instance()->loadSurveyWoundImage(survey))
                blockMainActions(false);
            else break;
        }
        ui->image->openImage(survey->image,survey->date.toString("dd.MM.yyyy hh:mm"));
        break;
    }
//    case WOUND_TYPE:
//        break;
//    case PATIENT_TYPE:
//        break;
//    case DOCTOR_TYPE:
//        break;
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(index);
    blockDatabaseActions(false);
    ui->databaseNameLineEdit->setReadOnly(false);
    switch (item->type()) {
    case SURVEY_TYPE:
    {
        Survey *survey = static_cast<Survey*>(item);
        ui->databaseNameLineEdit->setText(survey->date.toString("dd.MM.yyyy hh:mm"));
        ui->databaseNotesPlainTextEdit->setPlainText(survey->notes);
        break;
    }
    case WOUND_TYPE:
    {
        Wound *wound = static_cast<Wound*>(item);
        ui->databaseNameLineEdit->setText(wound->name);
        ui->databaseNotesPlainTextEdit->setPlainText(wound->notes);
        break;
    }
    case PATIENT_TYPE:
    {
        Patient *patient = static_cast<Patient*>(item);
        ui->databaseNameLineEdit->setText(patient->name);
        ui->databaseNotesPlainTextEdit->setPlainText(patient->notes);
        break;
    }
    case DOCTOR_TYPE:
    {
        Doctor *doctor = static_cast<Doctor*>(item);
        ui->databaseNameLineEdit->setText(doctor->name);
        ui->databaseNameLineEdit->setReadOnly(true);
        ui->actionDelete->setDisabled(true);
        ui->databaseNotesPlainTextEdit->setPlainText(doctor->notes);
        break;
    }
    }
}

void MainWindow::on_actionUpdate_triggered()
{
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(
                ui->treeView->selectionModel()->currentIndex());
    switch (item->type()) {
    case SURVEY_TYPE:
    {
        QMessageBox confirmMsgBox;
        confirmMsgBox.setText("Update survey");
        confirmMsgBox.setInformativeText("Are You shure in updating of current survey?");
        confirmMsgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        confirmMsgBox.setIcon(QMessageBox::Warning);
        int ret = confirmMsgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            Survey *typedItem = static_cast<Survey*>(item);
            typedItem->date = QDateTime::fromString(ui->databaseNameLineEdit->text(), "dd.MM.yyyy hh:mm");
            typedItem->setText(typedItem->date.toString("dd.MM.yyyy hh:mm"));
            typedItem->notes = ui->databaseNotesPlainTextEdit->toPlainText();
            typedItem->image = ImageManager::instance()->getImage().clone();
            typedItem->woundArea = ui->lineEditArea->text().toDouble();
            if(!DatabaseManager::instance()->update(typedItem))
            {
                QMessageBox msgBox;
                msgBox.setText("Update failed");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
            }
            ui->treeView->show();
        }
        break;
    }
    case WOUND_TYPE:
    {
        Wound *typedItem = static_cast<Wound*>(item);
        typedItem->name = ui->databaseNameLineEdit->text();
        typedItem->setText(typedItem->name);
        typedItem->notes = ui->databaseNotesPlainTextEdit->toPlainText();
        if(!DatabaseManager::instance()->update(typedItem))
        {
            QMessageBox msgBox;
            msgBox.setText("Update failed");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
        ui->treeView->show();
        break;
    }
    case PATIENT_TYPE:
    {
        Patient *typedItem = static_cast<Patient*>(item);
        typedItem->name = ui->databaseNameLineEdit->text();
        typedItem->setText(typedItem->name);
        typedItem->notes = ui->databaseNotesPlainTextEdit->toPlainText();
        if(!DatabaseManager::instance()->update(typedItem))
        {
            QMessageBox msgBox;
            msgBox.setText("Update failed");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
        ui->treeView->show();
        break;
    }
    case DOCTOR_TYPE: // Note that doctor's name can't be changed
    {
        Doctor *typedItem = static_cast<Doctor*>(item);
        typedItem->notes = ui->databaseNotesPlainTextEdit->toPlainText();
        if(!DatabaseManager::instance()->update(typedItem))
        {
            QMessageBox msgBox;
            msgBox.setText("Update failed");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
        ui->treeView->show();
        break;
    }
    }
}
