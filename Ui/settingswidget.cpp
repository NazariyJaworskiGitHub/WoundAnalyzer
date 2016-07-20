#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(ImageInterface* ptr, QWidget *parent) :
    QDialog(parent),
    _ptr(ptr),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    ui->polygonEdge->setStyleSheet("background-color: " +
                                   _ptr->polygonEdgeColor.name());
    ui->polygonColor->setStyleSheet("background-color: " +
                                    _ptr->polygonColor.name());
    ui->polygonTextColor->setStyleSheet("background-color: " +
                                    _ptr->polygonTextColor.name());
    ui->polygonEdgeThickness->setValue(_ptr->polygonEdgeThickness);
    ui->rulerColor->setStyleSheet("background-color: " +
                                  _ptr->rulerColor.name());
    ui->rulerNodesColor->setStyleSheet("background-color: " +
                                       _ptr->rulerNodesColor.name());
    ui->rulerTextColor->setStyleSheet("background-color: " +
                                       _ptr->rulerTextColor.name());
    ui->rulerThickness->setValue(_ptr->rulerThickness);
    ui->horizontalSlider->setValue(_ptr->transparency);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::_colorUpdate(QColor &_refToParentColor, QPushButton *_refToWidgetButton)
{
    QColor _color = QColorDialog::getColor(
                _refToParentColor,
                this,
                "Select color");
    if(_color.isValid())
    {
        _refToParentColor = _color;
        _refToWidgetButton->setStyleSheet("background-color: " + _color.name());
        _ptr->drawStuff();
    }
}

void SettingsWidget::on_polygonEdge_clicked()
{
    _colorUpdate(_ptr->polygonEdgeColor, ui->polygonEdge);
}

void SettingsWidget::on_polygonColor_clicked()
{
    _colorUpdate(_ptr->polygonColor, ui->polygonColor);
}

void SettingsWidget::on_rulerColor_clicked()
{
    _colorUpdate(_ptr->rulerColor, ui->rulerColor);
}

void SettingsWidget::on_rulerNodesColor_clicked()
{
    _colorUpdate(_ptr->rulerColor, ui->rulerColor);
}

void SettingsWidget::on_polygonEdgeThickness_valueChanged(int arg1)
{
    _ptr->polygonEdgeThickness = arg1;
    _ptr->drawStuff();
}

void SettingsWidget::on_rulerThickness_valueChanged(int arg1)
{
    _ptr->rulerThickness = arg1;
    _ptr->drawStuff();
}

void SettingsWidget::on_polygonTextColor_clicked()
{
    _colorUpdate(_ptr->polygonTextColor, ui->polygonTextColor);
}

void SettingsWidget::on_rulerTextColor_clicked()
{
    _colorUpdate(_ptr->rulerTextColor, ui->rulerTextColor);
}

void SettingsWidget::on_horizontalSlider_valueChanged(int value)
{
    _ptr->transparency = value;
    _ptr->drawStuff();
}
