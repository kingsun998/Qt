#include "multipleunion.h"
#include "ui_multipleunion.h"

MultipleUnion::MultipleUnion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultipleUnion)
{
    ui->setupUi(this);
}

MultipleUnion::~MultipleUnion()
{
    delete ui;
}
