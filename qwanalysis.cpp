#include "qwanalysis.h"
#include "ui_qwanalysis.h"

QWAnalysis::QWAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWAnalysis)
{
    ui->setupUi(this);
    ui->gridLayout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    setWindowTitle(QString("隔离开关操作机构箱内温湿度监测综合分析"));
}

QWAnalysis::~QWAnalysis()
{
    delete ui;
}

void QWAnalysis::TemperatureHight()
{
    ui->textEditTER->clear();
    ui->textEditTER->setText("<font color=\"#FF0000\">高温报警启动风扇</font> ");

}

void QWAnalysis::TemperatureNormal()
{
    ui->textEditTER->clear();
    ui->textEditTER->setText("<font color=\"#00FF00\">温度正常</font> ");
}
void QWAnalysis::TemperatureLow()
{
    ui->textEditTER->clear();
    ui->textEditTER->setText("<font color=\"#0000FF\">低温报警启动加热</font> ");
}

void QWAnalysis::HumidityHight()
{
    ui->textEditHUM->clear();
    ui->textEditHUM->setText("<font color=\"#FF0000\">高湿报警启动风吹机</font> ");
}
void QWAnalysis::HumidityNormal()
{
    ui->textEditHUM->clear();
    ui->textEditHUM->setText("<font color=\"#00FF00\">湿度正常</font> ");
}
