#include "qwparameterinf.h"
#include "ui_qwparameterinf.h"

#include "MainWindow.h"

#include <QDebug>


QWParameterInf::QWParameterInf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWParameterInf)
{
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    //添加标题
    setWindowTitle(QString("隔离开关操作机构箱内温湿度监测界面"));

    ui->horizontalLayout->setAlignment(Qt::AlignCenter);

    //设置表盘值改变时发出sendData信号，关联Compare槽函数
    connect(ui->dashTem2,SIGNAL(sendData(int)),this,SLOT(Compare()));
    connect(ui->dashHum2,SIGNAL(sendData(int)),this,SLOT(Compare()));


}


QWParameterInf::~QWParameterInf()
{
    delete ui;
}

//延时函数
void QWParameterInf::sleep(unsigned int msec)
{
    QTime reachTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

//温度数据同步
void QWParameterInf::TemData(int data)
{
    ui->dashTem2->setValue(data);
    ui->edtTemperature->setText(QString::number(data)+"℃");
}
//湿度数据同步
void QWParameterInf::HumData(int data)
{
    ui->dashHum2->setValue(data);
    ui->edtHumidity->setText(QString::number(data)+"%");
}

//比较当前值与阈值，并控制分析窗口的文字显示
void QWParameterInf::Compare()
{
    if(mySetting->HigtValue < ui->dashTem2->qValue)
    {
        myanalysis->TemperatureHight();
    }
    else if(mySetting->LowValue > ui->dashTem2->qValue)
        myanalysis->TemperatureLow();
    else
        myanalysis->TemperatureNormal();

    if(mySetting->HumValue < ui->dashHum2->qValue)
        myanalysis->HumidityHight();
    else
        myanalysis->HumidityNormal();
}

void QWParameterInf::on_btnParameterSetting_clicked()
{
    mySetting->exec();

}

void QWParameterInf::on_btnAnalsis_clicked()
{
    myanalysis->show();
}


