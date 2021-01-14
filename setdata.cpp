#include "setdata.h"
#include "ui_setdata.h"

SetData::SetData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetData)
{
    ui->setupUi(this);
    setWindowTitle("隔离开关操作机构扭矩监测参数设置");
}

SetData::~SetData()
{
    delete ui;
}

void SetData::on_btPreservation_clicked()
{
   setTorMax=ui->linsetTorMax->text().toInt(&ok,10);
   setAngMax=ui->linsetAngelMax->text().toInt(&ok,10);
   setCurMax=ui->linecurrentMax->text().toInt(&ok,10);
   setTorMin=ui->linsetTorMin->text().toInt(&ok,10);
   setAngMin=ui->linsetAngelMin->text().toInt(&ok,10);
   setCurMin=ui->linecurrentMin->text().toInt(&ok,10);
   emit MaxTorChange();
   isOk = true;
   this->hide();
}
