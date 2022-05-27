#include "mode_settings.h"
#include "ui_mode_settings.h"

mode_settings::mode_settings(QWidget *parent) :
    mode_base (parent),
    ui(new Ui::mode_settings)
{
    ui->setupUi(this);
}

mode_settings::~mode_settings()
{
    delete ui;
}

void mode_settings::reset(){

}
void mode_settings::init(){

}
