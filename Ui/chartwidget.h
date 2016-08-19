#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QDialog>

#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QTextCursor>
#include <QDesktopServices>

#include <qcustomplot/qcustomplot.h>
#include <qcustomplot/qcpdocumentobject.h>
#include <DataBase/databasemodel.h>

#include "Utilities/Logger/logger.h"

namespace Ui {
class ChartWidget;
}

class ChartWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ChartWidget(const Wound *wound, QWidget *parent = 0);
    ~ChartWidget();

private slots:
    void on_toolButton_clicked();

private:
    Ui::ChartWidget *ui;
    const Wound *wound;
};

#endif // CHARTWIDGET_H
