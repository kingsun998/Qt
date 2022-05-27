#include "mode_sending.h"
#include "ui_mode_sending.h"

mode_sending::mode_sending(QWidget *parent) :
   mode_base (parent),
    ui(new Ui::mode_sending)
{
    ui->setupUi(this);
}

mode_sending::~mode_sending()
{
    delete ui;
}

void mode_sending::reset(){

}
void mode_sending::init(){

}
