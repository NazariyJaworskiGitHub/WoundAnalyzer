#ifndef NODESUBITEMWIDGET
#define NODESUBITEMWIDGET

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QPoint>
#include <QIcon>
#include <QHBoxLayout>

class NodeSubItemWidget : public QWidget
{
    Q_OBJECT

    // should use the copy because QPolygon is QVector and pointers may change
    public : QPoint         node;
    private: QPushButton    *_deleteButton      = nullptr;
    private: QIcon          *_deleteButtonIcon  = nullptr;
    private: QLineEdit      *_xLineEdit         = nullptr;
    private: QLineEdit      *_yLineEdit         = nullptr;

    public : NodeSubItemWidget(QPoint node, QWidget *parent = 0):
        QWidget(parent),
        node(node)
    {
        _deleteButtonIcon = new QIcon("Icons/Delete.png");
        _deleteButton = new QPushButton(*_deleteButtonIcon,"",this);
        connect(_deleteButton,SIGNAL(clicked()),SLOT(on_pushButton_Delete_clicked()));

        _xLineEdit = new QLineEdit(QString::number(node.x()),this);
        _yLineEdit = new QLineEdit(QString::number(node.y()),this);

        QHBoxLayout *_layout = new QHBoxLayout();
        _layout->setContentsMargins(0,0,0,0);
        _layout->setSpacing(0);
        _layout->addWidget(_deleteButton);
        _layout->addWidget(_xLineEdit);
        _layout->addWidget(_yLineEdit);
        this->setLayout(_layout);
    }
    public : ~NodeSubItemWidget(){}

    public : Q_SLOT void update(QPoint newVal)
    {
        node = newVal;
        _xLineEdit->setText(QString::number(node.x()));
        _yLineEdit->setText(QString::number(node.y()));
    }

    public : Q_SLOT void on_pushButton_Delete_clicked()
    {
        Q_EMIT sDelete(this);
    }

        /// Catch this with parent object to destroy current object
    public : Q_SIGNAL void sDelete(NodeSubItemWidget* item);
};

#endif // NODESUBITEMWIDGET

