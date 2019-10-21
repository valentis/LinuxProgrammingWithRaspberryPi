#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_lineEdit_textChanged(const QString &arg1)
{
    ui->spinBox->blockSignals(true);
    ui->dial->setValue(arg1.toInt());
    ui->spinBox->blockSignals(false);
}
