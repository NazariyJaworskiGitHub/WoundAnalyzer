#include "chartwidget.h"
#include "ui_chartwidget.h"

ChartWidget::ChartWidget(const Wound *wound, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartWidget),
    wound(wound)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    QVector<double> _keyData;
    QVector<double> _valueData;

    ui->chart->addGraph();
    ui->chart->graph()->setName(wound->name + " dynamics");
    ui->chart->graph()->setBrush(QBrush(Qt::lightGray));
    ui->chart->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::white, 7));

    ui->chart->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->chart->xAxis->setDateTimeFormat("dd.MM\nyyyy\nhh:mm");
    ui->chart->xAxis->setLabel("Date");
    ui->chart->yAxis->setLabel("Wounded area (sm2)");

    ui->chart->legend->setVisible(true);

    for(int i=0; i<wound->rowCount(); ++i)
    {
        Survey *survey = static_cast<Survey*>(wound->child(i));
        _keyData.append(survey->date.toMSecsSinceEpoch()/1000.0);
        _valueData.append(survey->woundArea);
    }
    ui->chart->graph()->setData(_keyData, _valueData);

    ui->chart->setInteraction(QCP::iRangeDrag, true);
    ui->chart->setInteraction(QCP::iRangeZoom, true);

    ui->chart->rescaleAxes();
    ui->chart->replot();
}

ChartWidget::~ChartWidget()
{
    delete ui;
}

void ChartWidget::on_toolButton_clicked()
{
    Log::StaticLogger::instance() << "[Actions] Export_chart triggered\n";
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
        cursor.insertHtml(
                    "<h1><center>Wound Analysis Report [" +
                    QString::number(QDate::currentDate().day()) + "." +
                    QString::number(QDate::currentDate().month()) + "." +
                    QString::number(QDate::currentDate().year()) + "]</<center></h1>" +
                    "<p><b>Patient: </b> " + static_cast<Patient*>(wound->parent())->name + "</p><br>"
                    "<p><b>Patient notes: </b><pre>" + static_cast<Patient*>(wound->parent())->notes + "</pre></p><br>"
                    "<p><b>Wound: </b> " + wound->name + "</p><br>"
                    "<p><b>Wound notes: </b><pre>" + wound->notes + "</pre></p><br>");
        QString table =
                "<table cellspacing=0 cellpadding=2 style='border-width: 1px; border-style: solid; border-color: #000000'>"
                "<tr><th>Date</th>";
        for(int i=0; i<wound->rowCount(); ++i)
        {
            Survey *survey = static_cast<Survey*>(wound->child(i));
            table += "<td>" + survey->date.toString("dd.MM<br>yyyy<br>hh:mm") + "</td>";
        }
        table += "</tr><tr><th>Wounded<br>area (sm<sup>2</sup>)</th>";
        for(int i=0; i<wound->rowCount(); ++i)
        {
            Survey *survey = static_cast<Survey*>(wound->child(i));
            table += "<td>" + QString::number(survey->woundArea) + "</td>";
        }
        table += "</tr></table><br><br>";
        cursor.insertHtml(table);

        QCPDocumentObject *plotObjectHandler = new QCPDocumentObject(this);
        doc.documentLayout()->registerHandler(QCPDocumentObject::PlotTextFormat, plotObjectHandler);
        cursor.insertText(
                    QString(QChar::ObjectReplacementCharacter),
                    QCPDocumentObject::generatePlotFormat(
                        ui->chart,
                        printer.pageRect(QPrinter::Point).size().width(),
                        printer.pageRect(QPrinter::Point).size().width()/2.0));

        doc.print(&printer);

        bool _result = QDesktopServices::openUrl(QUrl::fromLocalFile(_fileName));
        if(_result)
            Log::StaticLogger::instance() << "[Utilities] results are shown\n";
        else
        {
            Log::StaticLogger::instance() << "[Utilities] <FAIL> results are not shown\n";
            QMessageBox msgBox;
            msgBox.setText("Fail");
            msgBox.setInformativeText("See LogFile for more details");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }
}
