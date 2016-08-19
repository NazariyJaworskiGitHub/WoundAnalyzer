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

    _mousePosition = new QLabel(this);
    _managementMode = new QLabel(this);

    ui->statusBar->addWidget(_mousePosition);
    ui->statusBar->addWidget(_managementMode);

    connect(ui->image,SIGNAL(mouseMoved_signal(QMouseEvent*)),
        this, SLOT(updateMousePositionStatus(QMouseEvent*)));

    connect(ui->image,SIGNAL(updateFilenameAndResetZoom_signal(QString)),
        this, SLOT(updateImageFileNameAndResetZoom(QString)));

    connect(ui->image,SIGNAL(updateManagementMode_signal(ImageInterface::ManagementMode)),
        this, SLOT(updateManagementModeStatus(ImageInterface::ManagementMode)));

    connect(ui->image,SIGNAL(updatePolygonArea_signal(double)),
        this, SLOT(updatePolygonArea(double)));

    _DBToolbar = new QToolBar(this);
    _DBToolbar->addAction(ui->actionAdd);
    _DBToolbar->addAction(ui->actionDelete);
    _DBToolbar->addAction(ui->actionUpdate);
    _DBToolbar->addAction(ui->actionDynamics);
    _DBToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->databaseHolderContents->layout()->addWidget(_DBToolbar);
    ui->databaseDateTimeEdit->hide();

    blockMainActions(true);
    blockDatabaseActions(true);

    /// \todo
    ui->doubleSpinBoxA->hide();
    ui->doubleSpinBoxB->hide();
    ui->doubleSpinBoxC->hide();
    ui->spinBoxD->hide();

    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_reselectDBItem()
{
    if(ui->treeView->model())
    {
        QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(
                    ui->treeView->selectionModel()->currentIndex());
        if(item->index().isValid())
        {
            ui->treeView->setCurrentIndex(item->index());
            on_treeView_clicked(item->index());
        }
    }
}

void MainWindow::_failMsg()
{
    QMessageBox msgBox;
    msgBox.setText("Fail");
    msgBox.setInformativeText("See LogFile for more details");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}

void MainWindow::updateMousePositionStatus(QMouseEvent *ev)
{
    _mousePosition->setText(QString::number(ev->x()) + "," + QString::number(ev->y()));
}

void MainWindow::updateManagementModeStatus(ImageInterface::ManagementMode mode)
{
    if(mode == ImageInterface::POLYGON_MODE)
        _managementMode->setText("[Polygon mode, click to add new polygon nodes]");
    else if(mode == ImageInterface::RULER_MODE)
        _managementMode->setText("[Ruler mode, click to add node]");
    else if(mode == ImageInterface::EDIT_MODE)
        _managementMode->setText("[Edit mode, LMB to drag nodes, RMB to delete polygon nodes or split edges]");
}

void MainWindow::updatePolygonArea(double area)
{
    ui->lineEditArea->setText(QString::number(area));
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
    ui->actionDynamics->setDisabled(b);
    ui->databaseNameLineEdit->setDisabled(b);
    ui->databaseNotesPlainTextEdit->setDisabled(b);
    ui->databaseDateTimeEdit->setDisabled(b);
}

void MainWindow::on_actionOpen_triggered()
{
    Log::StaticLogger::instance() << "[Actions] Open triggered\n";
    if(ImageManager::instance()->isImageOpened)
    {
        QMessageBox confirmMsgBox;
        confirmMsgBox.setText("Load survey");
        confirmMsgBox.setInformativeText("Are You shure in loading new survey?\nUnsaved changens will be lost!");
        confirmMsgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        confirmMsgBox.setIcon(QMessageBox::Information);
        int ret = confirmMsgBox.exec();
        if(ret == QMessageBox::Yes)
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
                _reselectDBItem();
            }
        }
    }
    else
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
            _reselectDBItem();
        }
    }
}

void MainWindow::on_actionPolygon_toggled(bool arg1)
{
    Log::StaticLogger::instance() << "[Actions] Polygon toggled\n";
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
    Log::StaticLogger::instance() << "[Actions] Ruler triggered\n";
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
    Log::StaticLogger::instance() << "[Actions] Clear triggered\n";
    ui->image->clearAll();
}

void MainWindow::on_actionAbout_triggered()
{
    Log::StaticLogger::instance() << "[Actions] About triggered\n";
    QMessageBox msgBox;
    msgBox.setText("<b>Wound Analyzer</b>");
    msgBox.setInformativeText("v0.03 by Nazariy Jaworski 2016 (c)");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::on_actionSettings_triggered()
{
    Log::StaticLogger::instance() << "[Actions] Settings triggered\n";
    SettingsWidget *_sw = new SettingsWidget(ui->image, this);
    _sw->exec();
}

void MainWindow::on_actionSave_triggered()
{
    Log::StaticLogger::instance() << "[Actions] Save triggered\n";
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
    Log::StaticLogger::instance() << "[Actions] Zoom_In triggered\n";
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
    Log::StaticLogger::instance() << "[Actions] Zoom_Out triggered\n";
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

void MainWindow::on_actionExport_results_triggered()
{    
    Log::StaticLogger::instance() << "[Actions] Export_results triggered\n";
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
                    QString::number(QDate::currentDate().day()) + "." +
                    QString::number(QDate::currentDate().month()) + "." +
                    QString::number(QDate::currentDate().year()) + "]</<center></h1>" +
                    "<p><b>Image: </b> " + _imageFileName + "</p><br>");
        cursor.insertImage(ImageManager::instance()->getImageAsQImage().scaled(
                               printer.pageRect(QPrinter::Point).size().width(),
                               printer.pageRect(QPrinter::Point).size().width(),
                               Qt::KeepAspectRatio));
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
        {
            Log::StaticLogger::instance() << "[Utilities] <FAIL> results are not shown\n";
            _failMsg();
        }
    }
}

void MainWindow::on_actionContext_triggered()
{
    Log::StaticLogger::instance() << "[Actions] Context triggered\n";
    bool _result = QDesktopServices::openUrl(QUrl::fromLocalFile("Wound analyzer context.pdf"));
    if(_result)
        Log::StaticLogger::instance() << "[Utilities] context file is opened\n";
    else
    {
        Log::StaticLogger::instance() << "[Utilities] <FAIL> context file is not opened\n";
        _failMsg();
    }
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
        ui->treeView->setCurrentIndex(DatabaseManager::instance()->DBrep->doctor->index());
        on_treeView_clicked(DatabaseManager::instance()->DBrep->doctor->index());
    }
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(index);
    switch (item->type()) {
    case SURVEY_TYPE:
    {
        if(ImageManager::instance()->isImageOpened)
        {
            QMessageBox confirmMsgBox;
            confirmMsgBox.setText("Load survey");
            confirmMsgBox.setInformativeText("Are You shure in loading new survey?\nUnsaved changens will be lost!");
            confirmMsgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
            confirmMsgBox.setIcon(QMessageBox::Information);
            int ret = confirmMsgBox.exec();
            if(ret == QMessageBox::Yes)
            {
                Survey *survey = static_cast<Survey*>(item);
                if(survey->image.empty())
                {
                    if(DatabaseManager::instance()->loadSurveyWoundImage(survey))
                        blockMainActions(false);
                    else
                    {
                        _failMsg();
                        break;
                    }
                }
                ui->image->openImage(survey->image,survey->date.toString("dd.MM.yyyy hh:mm"));
                ui->image->applyPolygonsAndRulerPoints(
                            survey->polygons,
                            survey->rulerPoints);
                ui->rulerFactorDoubleSpinBox->setValue(survey->rulerFactor);
                ImageManager::instance()->rulerFactor = survey->rulerFactor;
                ui->image->drawAll();
            }
        }
        else
        {
            Survey *survey = static_cast<Survey*>(item);
            if(survey->image.empty())
            {
                if(DatabaseManager::instance()->loadSurveyWoundImage(survey))
                    blockMainActions(false);
                else
                {
                    _failMsg();
                    break;
                }
            }
            ui->image->openImage(survey->image,survey->date.toString("dd.MM.yyyy hh:mm"));
            ui->image->applyPolygonsAndRulerPoints(
                        survey->polygons,
                        survey->rulerPoints);
            ui->rulerFactorDoubleSpinBox->setValue(survey->rulerFactor);
            ImageManager::instance()->rulerFactor = survey->rulerFactor;
            ui->image->drawAll();
        }
        break;
    }
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(index);
    blockDatabaseActions(false);
    ui->databaseNameLineEdit->show();
    ui->databaseDateTimeEdit->hide();
    ui->databaseNameLineEdit->setReadOnly(false);
    ui->databaseInfoLabel->setText("");
    switch (item->type()) {
    case SURVEY_TYPE:
    {
        Survey *survey = static_cast<Survey*>(item);
        ui->databaseNameLineEdit->hide();
        ui->actionAdd->setDisabled(true);
        ui->databaseDateTimeEdit->show();
        ui->databaseDateTimeEdit->setDateTime(survey->date);
        ui->databaseNotesPlainTextEdit->setPlainText(survey->notes);
        if(!ImageManager::instance()->isImageOpened)
        {
            ui->actionUpdate->setDisabled(true);
            ui->databaseInfoLabel->setText("Open image before updating survey\n[Double click to load survey]");
        }
        ui->actionAdd->setText("Add");
        ui->actionDelete->setText("Delete\nsurvey");
        ui->actionUpdate->setText("Update\nsurvey");
        ui->actionDynamics->setDisabled(true);
        break;
    }
    case WOUND_TYPE:
    {
        Wound *wound = static_cast<Wound*>(item);
        ui->databaseNameLineEdit->setText(wound->name);
        ui->databaseNotesPlainTextEdit->setPlainText(wound->notes);
        if(!ImageManager::instance()->isImageOpened)
        {
            ui->actionAdd->setDisabled(true);
            ui->databaseInfoLabel->setText("Open image before adding new survey");
        }
        ui->actionAdd->setText("Add\nsurvey");
        ui->actionDelete->setText("Delete\nwound");
        ui->actionUpdate->setText("Update\nwound");
        if(wound->hasChildren())
            ui->actionDynamics->setEnabled(true);
        else
            ui->actionDynamics->setDisabled(true);
        break;
    }
    case PATIENT_TYPE:
    {
        Patient *patient = static_cast<Patient*>(item);
        ui->databaseNameLineEdit->setText(patient->name);
        ui->databaseNotesPlainTextEdit->setPlainText(patient->notes);
        ui->actionAdd->setText("Add\nwound");
        ui->actionDelete->setText("Delete\npatient");
        ui->actionUpdate->setText("Update\npatient");
        ui->actionDynamics->setDisabled(true);
        break;
    }
    case DOCTOR_TYPE:
    {
        Doctor *doctor = static_cast<Doctor*>(item);
        ui->databaseNameLineEdit->setText(doctor->name);
        ui->databaseNameLineEdit->setReadOnly(true);
        ui->actionDelete->setDisabled(true);
        ui->databaseInfoLabel->setText("Doctor can't be deleted");
        ui->databaseNotesPlainTextEdit->setPlainText(doctor->notes);
        ui->actionAdd->setText("Add\npatient");
        ui->actionDelete->setText("Delete");
        ui->actionUpdate->setText("Update\ndoctor");
        ui->actionDynamics->setDisabled(true);
        break;
    }
    }
}

void MainWindow::on_actionUpdate_triggered()
{
    Log::StaticLogger::instance() << "[Actions] Update triggered\n";
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(
                ui->treeView->selectionModel()->currentIndex());
    switch (item->type()) {
    case SURVEY_TYPE:
    {
        QMessageBox confirmMsgBox;
        confirmMsgBox.setText("Update survey");
        confirmMsgBox.setInformativeText("Are You shure in updating of current survey?");
        confirmMsgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        confirmMsgBox.setIcon(QMessageBox::Information);
        int ret = confirmMsgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            Survey *typedItem = static_cast<Survey*>(item);
            typedItem->date = ui->databaseDateTimeEdit->dateTime();
            typedItem->setText(typedItem->date.toString("dd.MM.yyyy hh:mm") + " " + ((ui->image->woundsArea != 0) ? (QString::number(ui->image->woundsArea) + "sm2") : ("")));
            typedItem->notes = ui->databaseNotesPlainTextEdit->toPlainText();
            typedItem->image = ImageManager::instance()->getImage().clone();

            double currentZoom = ImageManager::instance()->zoomFactor;
            ui->image->zoom(1.0-currentZoom);

            typedItem->applyPolygonsAndRulerPoints(
                        ui->image->polygons,
                        ui->image->rulerPoints);

            ui->image->zoom(currentZoom-1.0);

            typedItem->rulerFactor = ui->rulerFactorDoubleSpinBox->value();
            typedItem->woundArea = ui->image->woundsArea;
            if(!DatabaseManager::instance()->update(typedItem)) _failMsg();
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
        if(!DatabaseManager::instance()->update(typedItem)) _failMsg();
        ui->treeView->show();
        break;
    }
    case PATIENT_TYPE:
    {
        Patient *typedItem = static_cast<Patient*>(item);
        typedItem->name = ui->databaseNameLineEdit->text();
        typedItem->setText(typedItem->name);
        typedItem->notes = ui->databaseNotesPlainTextEdit->toPlainText();
        if(!DatabaseManager::instance()->update(typedItem)) _failMsg();
        ui->treeView->show();
        break;
    }
    case DOCTOR_TYPE: // Note that doctor's name can't be changed
    {
        Doctor *typedItem = static_cast<Doctor*>(item);
        typedItem->notes = ui->databaseNotesPlainTextEdit->toPlainText();
        if(!DatabaseManager::instance()->update(typedItem)) _failMsg();
        ui->treeView->show();
        break;
    }
    }
}

void MainWindow::on_actionAdd_triggered()
{
    Log::StaticLogger::instance() << "[Actions] Add triggered\n";
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(
                ui->treeView->selectionModel()->currentIndex());
    switch (item->type()) {
    case WOUND_TYPE:
    {
        Wound *typedItem = static_cast<Wound*>(item);
        Survey *newTarget = DatabaseManager::instance()->add(typedItem);
        if(newTarget)
        {
            ui->treeView->setCurrentIndex(newTarget->index());
            on_treeView_clicked(newTarget->index());

            double currentZoom = ImageManager::instance()->zoomFactor;
            ui->image->zoom(1.0-currentZoom);

            newTarget->applyPolygonsAndRulerPoints(
                        ui->image->polygons,
                        ui->image->rulerPoints);

            ui->image->zoom(currentZoom-1.0);

            newTarget->setText(newTarget->date.toString("dd.MM.yyyy hh:mm") + " " + ((ui->image->woundsArea != 0) ? (QString::number(ui->image->woundsArea) + "sm2") : ("")));
            newTarget->rulerFactor = ui->rulerFactorDoubleSpinBox->value();
            newTarget->woundArea = ui->image->woundsArea;
            if(!DatabaseManager::instance()->update(newTarget)) _failMsg();
            ui->treeView->show();
        }
        else _failMsg();
        break;
    }
    case PATIENT_TYPE:
    {
        Patient *typedItem = static_cast<Patient*>(item);
        Wound *newTarget = DatabaseManager::instance()->add(typedItem);
        if(newTarget)
        {
            ui->treeView->setCurrentIndex(newTarget->index());
            on_treeView_clicked(newTarget->index());
        }
        else _failMsg();
        break;
    }
    case DOCTOR_TYPE:
    {
        Doctor *typedItem = static_cast<Doctor*>(item);
        Patient *newTarget = DatabaseManager::instance()->add(typedItem);
        if(newTarget)
        {
            ui->treeView->setCurrentIndex(newTarget->index());
            on_treeView_clicked(newTarget->index());
        }
        else _failMsg();
        break;
    }
    }
}

void MainWindow::on_actionDelete_triggered()
{
    Log::StaticLogger::instance() << "[Actions] Delete triggered\n";
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(
                ui->treeView->selectionModel()->currentIndex());
    switch (item->type()) {
    case SURVEY_TYPE:
    {
        QMessageBox confirmMsgBox;
        confirmMsgBox.setText("Delete survey");
        confirmMsgBox.setInformativeText("Are You shure in deleting of current survey?");
        confirmMsgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        confirmMsgBox.setIcon(QMessageBox::Information);
        int ret = confirmMsgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            Survey *typedItem = static_cast<Survey*>(item);
            Wound *parent = DatabaseManager::instance()->del(typedItem);
            if(parent)
            {
                ui->treeView->setCurrentIndex(parent->index());
                on_treeView_clicked(parent->index());
            }
            else _failMsg();
        }
        break;
    }
    case WOUND_TYPE:
    {
        QMessageBox confirmMsgBox;
        confirmMsgBox.setText("Delete wound");
        confirmMsgBox.setInformativeText("Are You shure in deleting of current wound?");
        confirmMsgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        confirmMsgBox.setIcon(QMessageBox::Information);
        int ret = confirmMsgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            Wound *typedItem = static_cast<Wound*>(item);
            Patient *parent = DatabaseManager::instance()->del(typedItem);
            if(parent)
            {
                ui->treeView->setCurrentIndex(parent->index());
                on_treeView_clicked(parent->index());
            }
            else _failMsg();
        }
        break;
    }
    case PATIENT_TYPE:
    {
        QMessageBox confirmMsgBox;
        confirmMsgBox.setText("Delete patient");
        confirmMsgBox.setInformativeText("Are You shure in deleting of current patient?");
        confirmMsgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        confirmMsgBox.setIcon(QMessageBox::Information);
        int ret = confirmMsgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            Patient *typedItem = static_cast<Patient*>(item);
            Doctor *parent = DatabaseManager::instance()->del(typedItem);
            if(parent)
            {
                ui->treeView->setCurrentIndex(parent->index());
                on_treeView_clicked(parent->index());
            }
            else _failMsg();
        }
        break;
    }
    }
}

void MainWindow::on_actionDynamics_triggered()
{
    Log::StaticLogger::instance() << "[Actions] Dynamics triggered\n";
    QStandardItem *item = static_cast<QStandardItemModel*>(ui->treeView->model())->itemFromIndex(
                ui->treeView->selectionModel()->currentIndex());
    ChartWidget *chW = new ChartWidget(static_cast<Wound*>(item), this);
    chW->exec();
}
