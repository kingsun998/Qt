#include "autopacking.h"
#include "ui_autopacking.h"
#include "qdebug.h"

autoPacking::autoPacking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoPacking)
{
    ui->setupUi(this);

    step_one = new autopack_step_one();
    ui->tabWidget->addTab(step_one,"步骤1");

    step_two = new autopack_step_two();
    ui->tabWidget->addTab(step_two,"步骤2");

    step_three = new autopack_step_three();
    ui->tabWidget->addTab(step_three,"步骤3");


}

autoPacking::~autoPacking()
{
    delete ui;
}
