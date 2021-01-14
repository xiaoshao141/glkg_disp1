#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ctemprmtp.h"

#include "modbuspro.h"
#include "sgmdb.h"
#include "devwave.h"
#include "devoper.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initTerminal();

    m_WaveFile = g_Devwave;

    connect(ui->dashtem1,SIGNAL(sendData(int)),this,SLOT(changeTem(int)));
    connect(ui->dashHum1,SIGNAL(sendData(int)),this,SLOT(changeHum(int)));

    connect(series1,&QSplineSeries::pressed,this,&MainWindow::axisY_angel);
    connect(series2,&QSplineSeries::pressed,this,&MainWindow::axisY_current);

    connect(ui->widget2->mytorque->m_setdata,SIGNAL(MaxTorChange()),this,SLOT(updateChartTor()));
    connect(ui->widget3->mytorque->m_setdata,SIGNAL(MaxTorChange()),this,SLOT(updateChartTor1()));

    connect(m_timer,SIGNAL(timeout()), this, SLOT(m_timerSlot()));
    m_timer->start(1000);

    connect(m_wavetimer,SIGNAL(timeout()), this, SLOT(rtWaveTimer()));
    m_wavetimer->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTerminal()
{
    //将label文字居中显示
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
    //将表盘居中显示
    ui->verticalLayout_5->setAlignment(Qt::AlignCenter);
    ui->verticalLayout_6->setAlignment(Qt::AlignCenter);

    //扭矩窗口居中
    ui->groupBox->setAlignment(Qt::AlignCenter);

    //电流窗口曲线图居中显示
    ui->groupBox_2->setAlignment(Qt::AlignCenter);

    //窗口最大化
   setWindowState(Qt::WindowMaximized);

    //设置窗口的qss样式
    setWindowStyle();

    GetDataFromWaveFile();

    //初始化扭矩和电流的波形图表格
    creatChart();

}

void MainWindow::m_timerSlot()
{
   GetTemFromTab();
   GetHumFromTab();
   GetDataFromWaveFile();

   if (g_aTempImg.size().width() > 100)
   {
        ui->label_3->setPixmap(QPixmap::fromImage(g_aTempImg.scaled(ui->label_3->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
   }
   if (g_bTempImg.size().width() > 100)
   {
       ui->label_4->setPixmap(QPixmap::fromImage(g_bTempImg.scaled(ui->label_4->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
   }
   if (g_cTempImg.size().width() > 100)
   ui->label_5->setPixmap(QPixmap::fromImage(g_cTempImg.scaled(ui->label_5->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

   updateSwState();
}

void MainWindow::rtWaveTimer()
{
    updateChart();
}

//从数据库中获取温度，湿度值
int MainWindow::GetTemFromTab()
{
    int ret = -1;
    float Tvalue =0;
    g_Devoper->getDevDBObjData(ADDR_TER,&Tvalue);
    ui->dashtem1->setValue((int)Tvalue);
    ret=1;
    return ret;
}

int MainWindow::GetHumFromTab()
{
    int ret = -1;
    float Hvalue=0;
    g_Devoper->getDevDBObjData(ADDR_HUM,&Hvalue);
    ui->dashHum1->setValue((int)Hvalue);
    ret=1;
    return ret;
}

//从录波文件中读取数据并在界面上显示波形（扭矩，电流，角度）
int MainWindow::GetDataFromWaveFile()
{
    WaveData_t waveData;
    QString fileName = g_Devwave->WaveFileCurrent();

    m_WaveFile->WaveFileExport(fileName,&waveData);

    ui->widget2->mytorque->SaveWavedata(&waveData);

    ui->widget3->mytorque->SaveWavedata(&waveData);

    ui->widget2->mytorque->dataPlot_tor(waveData.Tor);
    ui->widget2->mytorque->dataPlot_ang(waveData.Ang);
    ui->widget2->mytorque->dataPlot_cur(waveData.Cur);

    ui->widget3->mytorque->dataPlot_tor(waveData.Tor);
    ui->widget3->mytorque->dataPlot_ang(waveData.Ang);
    ui->widget3->mytorque->dataPlot_cur(waveData.Cur);

    return 1;
}

//温度表盘值的改变
void MainWindow::changeTem(int data)
{

  //  修改子窗口的温度表盘上的值，使其与主窗口数值保持同步
    ui->widget1->myparameter->TemData(data);

}
//湿度表盘值的改变
void MainWindow::changeHum(int data)
{

  //  修改子窗口的湿度表盘上的值，使其与主窗口数值保持同步
    ui->widget1->myparameter->HumData(data);

}

void MainWindow::creatChart()
{
    //绘制扭矩波形图表格
    chartTor->setTitle("");
    ui->chartview->setChart(chartTor);
    ui->chartview->setRenderHint(QPainter::Antialiasing);

    series->setName("扭矩");
    series->setPointsVisible(true);

    QMargins marg(10, 10, 10, 10);
    chartTor->setMargins(marg);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(Qt::green);
    series->setPen(pen);

    axisX->setRange(0, 15);//设置坐标范围
    axisX->setLabelFormat("%d");
    axisX->setTickCount(16);//主分隔个数
    axisX->setMinorTickCount(0);
    axisX->setTitleText("时间(s)");

    axisY->setRange(0, 400);
    axisY->setLabelFormat("%d");
    axisY->setTickCount(5);
    axisY->setMinorTickCount(0);
    axisY->setTitleText("扭矩(N*m)");

    //绘制角度、电流波形图表格
    chartAnlCur->setTitle("");
    ui->chartview1->setChart(chartAnlCur);
    ui->chartview1->setRenderHint(QPainter::Antialiasing);

    series2->setName("电流");
    series2->setPointsVisible(true);

    chartAnlCur->setMargins(marg);

    QPen p2;
    p2.setStyle(Qt::SolidLine);
    p2.setWidth(1);
    p2.setColor(Qt::blue);
    series2->setPen(p2);

    axisX1->setRange(0, 15);
    axisX1->setLabelFormat("%d");
    axisX1->setTickCount(16);
    axisX1->setMinorTickCount(0);
    axisX1->setTitleText("时间(s)");

    axisY1->setRange(0,3.0);
    axisY1->setLabelFormat("%.2f");
    axisY1->setTickCount(4);
    axisY1->setMinorTickCount(0);
    axisY1->setTitleText("电流(A)");

    if(series != NULL)
    {
        chartTor->addSeries(series);
        chartTor->setAxisX(axisX, series);//设置X坐标
        chartTor->setAxisY(axisY, series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    if(series2 != NULL)
    {
        chartAnlCur->addSeries(series2);
        chartAnlCur->setAxisX(axisX1,series2);
        chartAnlCur->setAxisY(axisY1, series2);
        series2->attachAxis(axisX1);
        series2->attachAxis(axisY1);
    }
}

//更新坐标轴范围
void MainWindow::updateChartTor()
{
    return;
    int MaxTor=0,MaxAng=0,MaxCur=0,
    MinTor=0,MinAng=0,MinCur=0;

    MaxTor = ui->widget2->mytorque->m_setdata->setTorMax;
    MaxAng = ui->widget2->mytorque->m_setdata->setAngMax;
    MaxCur = ui->widget2->mytorque->m_setdata->setCurMax;
    MinTor = ui->widget2->mytorque->m_setdata->setTorMin;
    MinAng = ui->widget2->mytorque->m_setdata->setAngMin;
    MinCur = ui->widget2->mytorque->m_setdata->setCurMin;

    axisY->setMax(MaxTor);
    axisY1->setMax(MaxAng);
    axisY2->setMax(MaxCur);
    axisY->setMin(MinTor);
    axisY1->setMin(MinAng);
    axisY2->setMin(MinCur);

    ui->widget2->mytorque->axisY->setMax(MaxTor);
    ui->widget2->mytorque->axisY1->setMax(MaxAng);
    ui->widget2->mytorque->axisY2->setMax(MaxCur);
    ui->widget2->mytorque->axisY->setMin(MinTor);
    ui->widget2->mytorque->axisY1->setMin(MinAng);
    ui->widget2->mytorque->axisY2->setMin(MinCur);

    ui->widget2->mytorque->analysis->axisY1->setMax(MaxTor);
    ui->widget2->mytorque->analysis->axisY2->setMax(MaxAng);
    ui->widget2->mytorque->analysis->axisY3->setMax(MaxCur);
    ui->widget2->mytorque->analysis->axisY1->setMin(MinTor);
    ui->widget2->mytorque->analysis->axisY2->setMin(MinAng);
    ui->widget2->mytorque->analysis->axisY3->setMin(MinCur);
}
void MainWindow::updateChartTor1()
{
    return;
    int MaxTor1=0,MaxAng1=0,MaxCur1=0,
    MinTor1=0,MinAng1=0,MinCur1=0;

    MaxTor1 = ui->widget3->mytorque->m_setdata->setTorMax;
    MaxAng1 = ui->widget3->mytorque->m_setdata->setAngMax;
    MaxCur1 = ui->widget3->mytorque->m_setdata->setCurMax;
    MinTor1 = ui->widget3->mytorque->m_setdata->setTorMin;
    MinAng1 = ui->widget3->mytorque->m_setdata->setAngMin;
    MinCur1 = ui->widget3->mytorque->m_setdata->setCurMin;

    axisY->setMax(MaxTor1);
    axisY1->setMax(MaxAng1);
    axisY2->setMax(MaxCur1);
    axisY->setMin(MinTor1);
    axisY1->setMin(MinAng1);
    axisY2->setMin(MinCur1);

    ui->widget3->mytorque->axisY->setMax(MaxTor1);
    ui->widget3->mytorque->axisY1->setMax(MaxAng1);
    ui->widget3->mytorque->axisY2->setMax(MaxCur1);
    ui->widget3->mytorque->axisY->setMin(MinTor1);
    ui->widget3->mytorque->axisY1->setMin(MinAng1);
    ui->widget3->mytorque->axisY2->setMin(MinCur1);

    ui->widget3->mytorque->analysis->axisY1->setMax(MaxTor1);
    ui->widget3->mytorque->analysis->axisY2->setMax(MaxAng1);
    ui->widget3->mytorque->analysis->axisY3->setMax(MaxCur1);
    ui->widget3->mytorque->analysis->axisY1->setMin(MinTor1);
    ui->widget3->mytorque->analysis->axisY2->setMin(MinAng1);
    ui->widget3->mytorque->analysis->axisY3->setMin(MinCur1);
}


void MainWindow::axisY_angel()
{
   axisY1->setLabelsVisible(true);//量程可见性
   axisY1->setTitleVisible(true);//坐标轴可见性
   axisY1->setVisible(true);//坐标轴可见性
   axisY2->setLabelsVisible(false);
   axisY2->setTitleVisible(false);
   axisY2->setVisible(false);
}

void MainWindow::axisY_current()
{
    axisY2->setLabelsVisible(true);
    axisY2->setTitleVisible(true);
    axisY2->setVisible(true);
    axisY1->setLabelsVisible(false);
    axisY1->setTitleVisible(false);
    axisY1->setVisible(false);
}

void MainWindow::updateSeriesTor(uint16_t* val, uint32_t idx)
{
    int dataNum = 0;
    static uint32_t lastIndex = 0;

    dataNum = (idx - lastIndex);

    /*@brief 20s 40*500ms */
    if(idx <= RT_WAVE_NUM && dataNum > 0)
    {
        for (int i = 0; i < dataNum; i++)
        {
            if(idx != RT_WAVE_NUM)
                series->append(((((i+lastIndex)%RT_WAVE_NUM)*15.0)/RT_WAVE_NUM) , val[lastIndex+i]);
            else
                series->append(15.0, val[RT_WAVE_NUM-1]);
        }

        lastIndex += dataNum;
    }

    if(idx >= RT_WAVE_NUM)
        lastIndex = 0;
}

void MainWindow::updateSeriesAng(uint16_t* val, uint32_t idx)
{
    int dataNum = 0;
    static uint32_t lastIndex = 0;
    dataNum = (idx - lastIndex);

    /*@brief 20s 40*500ms */
    if(idx <= RT_WAVE_NUM && dataNum > 0)
    {
        for (int i = 0; i < dataNum; i++)
        {
            if(idx != RT_WAVE_NUM)
                series1->append(((((i+lastIndex)%RT_WAVE_NUM)*15.0)/RT_WAVE_NUM) , val[lastIndex+i]);
            else
                series1->append(15.0 , val[RT_WAVE_NUM-1]);
        }

        lastIndex += dataNum;
    }

    if(idx >= RT_WAVE_NUM)
        lastIndex = 0;
}

void MainWindow::updateSeriesCur(uint16_t* val, uint32_t idx)
{
    int dataNum = 0;
    static uint32_t lastIndex = 0;

    dataNum = (idx - lastIndex);

    /*@brief 20s 40*500ms */
    if(idx <= RT_WAVE_NUM && dataNum > 0)
    {
        for (int i = 0; i < dataNum; i++)
        {
            if(idx != RT_WAVE_NUM)
                series2->append(((((i+lastIndex)%RT_WAVE_NUM)*15.0)/RT_WAVE_NUM) , val[lastIndex+i]/10.0);
            else
                series2->append(15.0 , val[RT_WAVE_NUM-1]/10.0);
        }

        lastIndex += dataNum;
    }

    if(idx >= RT_WAVE_NUM)
        lastIndex = 0;
}

void MainWindow::updateChart()
{
    uint32_t torIdx, angIdx, curIdx = 0;
    QPen pen;
    WaveData_t rtWave;

    float swWaveState = 0.0;
    g_Devoper->getDevDBObjData(YX_SWWAVE_ADDR, &swWaveState);

    if((bool)swWaveState == true && !g_Devwave->isStartWave())
    {
        series->clear();
        series2->clear();

        pen = series->pen();
        pen.setWidth(1);
        series->setPen(pen);

        pen = series2->pen();
        pen.setWidth(1);
        series2->setPen(pen);

        g_Devwave->startRtWave();
    }

    if(g_Devwave->isStartWave())
    {
        g_Devwave->getWave(&rtWave);
        g_Devwave->getIdx(&torIdx, &angIdx, &curIdx);

        /* @brief For Test.
        memset(&rtWave, 0, sizeof(WaveData_t));
        QString fileName = g_Devwave->WaveFileCurrent();
        m_WaveFile->WaveFileExport(fileName,&rtWave);
        torIdx = 1023;
        angIdx = 1023;
        curIdx = 1023;*/

        if((bool)swWaveState == false && \
           (torIdx>RT_WAVE_NUM) && (angIdx>RT_WAVE_NUM) && (curIdx>RT_WAVE_NUM))
        {
            g_Devwave->stopRtWave();

            pen = series->pen();
            pen.setWidth(2);
            series->setPen(pen);

            pen = series2->pen();
            pen.setWidth(2);
            series2->setPen(pen);
        }
        else
        {
            updateSeriesTor(rtWave.Tor, torIdx);
            updateSeriesAng(rtWave.Ang, angIdx);
            updateSeriesCur(rtWave.Cur, curIdx);
        }
    }
}

void MainWindow::updateSwState()
{
    float swOnState = 0.0;
    float swOffState = 0.0;

    g_Devoper->getDevDBObjData(YX_SWON_ADDR, &swOnState);
    g_Devoper->getDevDBObjData(YX_SWOFF_ADDR, &swOffState);

    if(swOnState && !swOffState)
        g_Devwave->setSwState(false);
    else if(!swOnState && swOffState)
        g_Devwave->setSwState(true);
}


//设置窗口的qss样式
void MainWindow::setWindowStyle()
{
    QFile file(":/qss/qss/blue.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    this->setStyleSheet(styleSheet);
    file.close();
}

void MainWindow::sleep(unsigned int msec)
{
    QTime reachTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
