#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QDialog>
#include <QColor>
#include <QColorDialog>

#include "Ui/imageinterface.h"

namespace Ui {
class SettingsWidget;
}

// Interact with ImageInterface through parent()->ui->label
class SettingsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWidget(ImageInterface* ptr, QWidget *parent = 0);
    ~SettingsWidget();

private: ImageInterface* _ptr = nullptr;
private: void _colorUpdate(QColor &_refToParentColor, QPushButton *_refToWidgetButton);

private Q_SLOTS:
    void on_polygonEdge_clicked();

    void on_polygonColor_clicked();

    void on_rulerColor_clicked();

    void on_rulerNodesColor_clicked();

    void on_polygonEdgeThickness_valueChanged(int arg1);

    void on_rulerThickness_valueChanged(int arg1);

    void on_polygonTextColor_clicked();

    void on_rulerTextColor_clicked();

private:
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
