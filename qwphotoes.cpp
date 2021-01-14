#include "qwphotoes.h"
#include "ui_qwphotoes.h"

#include "ctemprmtp.h"
#include "ctemmodbus.h"

QWPhotoes::QWPhotoes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWPhotoes)
{
    ui->setupUi(this);
    //添加标题
    setWindowTitle(QString("隔离开关主导电回路红外测温监控"));

    m_pTimer = new QTimer(this);

    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timerOut()));
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timerOut()));

    m_pTimer->start(1000);
}

QWPhotoes::~QWPhotoes()
{
    delete ui;
}

void QWPhotoes::on_btnParameterSetting_clicked()
{
    myPHParameter->exec();
}

void QWPhotoes::on_btnAnalsis_clicked()
{
    myPHAnalysis->exec();
}

void QWPhotoes::timerOut()
{
    if (g_aTempImg.size().width() > 100)
    {
         ui->label->setPixmap(QPixmap::fromImage(g_aTempImg.scaled(ui->label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    }
    if (g_bTempImg.size().width() > 100)
    {
        ui->label_2->setPixmap(QPixmap::fromImage(g_bTempImg.scaled(ui->label_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    }
    if (g_cTempImg.size().width() > 100)
    ui->label_3->setPixmap(QPixmap::fromImage(g_cTempImg.scaled(ui->label_3->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

//    ui->lineATem->setText(QString::number(aTemp, 'f', 1));
//    ui->lineBTem->setText(QString::number(bTemp, 'f', 1));
//    ui->lineCTem->setText(QString::number(cTemp, 'f', 1));
    TemCompare();
}

void QWPhotoes::TemCompare()
{
    if(aTemp>0 && aTemp<myPHParameter->OverTemWarn)
    {
        myPHAnalysis->aTemNormal();
    }
    else if(aTemp>myPHParameter->OverTemWarn && aTemp<myPHParameter->HighTemWarn)
    {
        myPHAnalysis->aOverTemWarn();
    }
    else if(aTemp>myPHParameter->HighTemWarn)
    {
        myPHAnalysis->aHighTemWarn();
    }

    if(bTemp>0 && bTemp<myPHParameter->OverTemWarn)
    {
        myPHAnalysis->bTemNormal();
    }
    else if(bTemp>myPHParameter->OverTemWarn && bTemp<myPHParameter->HighTemWarn)
    {
        myPHAnalysis->bOverTemWarn();
    }
    else if(bTemp>myPHParameter->HighTemWarn)
    {
        myPHAnalysis->bHighTemWarn();
    }

    if(cTemp>0 && cTemp<myPHParameter->OverTemWarn)
    {
        myPHAnalysis->cTemNormal();
    }
    else if(cTemp>myPHParameter->OverTemWarn && cTemp<myPHParameter->HighTemWarn)
    {
        myPHAnalysis->cOverTemWarn();
    }
    else if(cTemp>myPHParameter->HighTemWarn)
    {
        myPHAnalysis->cHighTemWarn();
    }
}
