#include "qphanalysis.h"
#include "ui_qphanalysis.h"

QPHAnalysis::QPHAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPHAnalysis)
{
    ui->setupUi(this);
    setWindowTitle(QString("隔离开关主导电回路红外测温监测综合分析"));
}

QPHAnalysis::~QPHAnalysis()
{
    delete ui;
}

void QPHAnalysis::aTemNormal()
{
    ui->textEditA->clear();
    ui->textEditA->setText("<font color=\"#00FF00\">温度正常</font> ");
}
void QPHAnalysis::aOverTemWarn()
{
     ui->textEditA->clear();
    ui->textEditA->setText("<font color=\"#FF00FF\">超温告警</font> ");
}
void QPHAnalysis::aHighTemWarn()
{
     ui->textEditA->clear();
    ui->textEditA->setText("<font color=\"#FF0000\">严重高温告警</font> ");
}

void QPHAnalysis::bTemNormal()
{
     ui->textEditB->clear();
    ui->textEditB->setText("<font color=\"#00FF00\">温度正常</font> ");
}
void QPHAnalysis::bOverTemWarn()
{
    ui->textEditB->clear();
    ui->textEditB->setText("<font color=\"#FF00FF\">超温告警</font> ");
}
void QPHAnalysis::bHighTemWarn()
{
    ui->textEditB->clear();
    ui->textEditB->setText("<font color=\"#FF0000\">严重高温告警</font> ");
}

void QPHAnalysis::cTemNormal()
{
    ui->textEditC->clear();
    ui->textEditC->setText("<font color=\"#00FF00\">温度正常</font> ");
}
void QPHAnalysis::cOverTemWarn()
{
    ui->textEditC->clear();
    ui->textEditC->setText("<font color=\"#FBCC24\">超温告警</font> ");
}
void QPHAnalysis::cHighTemWarn()
{
    ui->textEditC->clear();
    ui->textEditC->setText("<font color=\"#FF0000\">严重高温告警</font> ");
}


