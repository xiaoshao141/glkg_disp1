#include "qwparametersave.h"
#include "ui_qwparametersave.h"

#include "mainwindow.h"
#include "devoper.h"
#include "sgmdb.h"

#include <QSqlQuery>
#include <QDebug>

QWParameterSave::QWParameterSave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWParameterSave)
{
    ui->setupUi(this);
    ui->verticalLayout_2->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    setWindowTitle(QString("隔离开关操作机构箱内温湿度监测参数设置"));

    init();
}

QWParameterSave::~QWParameterSave()
{
    delete ui;
    delete configIni;
}

void QWParameterSave::init()
{
    HigtValue = configIni->value("TemHum/HigtValue").toInt();
    LowValue =  configIni->value("TemHum/LowValue").toInt();
    HumValue =  configIni->value("TemHum/HumValue").toInt();

    ui->lineEditH->setText(QString::number(HigtValue));
    ui->lineEditL->setText(QString::number(LowValue));
    ui->lineEditHum->setText(QString::number(HumValue));

}

//修改高温、低温和湿度告警阈值，与数据库装置定值的数据同步
void QWParameterSave::on_btnParameterSave_clicked()
{
        HigtValue = ui->lineEditH->text().toInt();
        LowValue = ui->lineEditL->text().toInt();
        HumValue = ui->lineEditHum->text().toInt();

        int r=0,s=0,t=0;
        if(r=g_Devoper->setDevDBObjData(ADDR_HigtWARN,&HigtValue))
            qDebug()<<"ADDR_HigtWARN";
        if(s=g_Devoper->setDevDBObjData(ADDR_LOWWARN,&LowValue))
            qDebug()<<"ADDR_LOWWARN";
        if(t=g_Devoper->setDevDBObjData(ADDR_HUMWARN,&HumValue))
            qDebug()<<"ADDR_HUMWARN";

        configIni->setValue("TemHum/HigtValue",HigtValue);
        configIni->setValue("TemHum/LowValue",LowValue);
        configIni->setValue("TemHum/HumValue",HumValue);
       this->hide();
}

