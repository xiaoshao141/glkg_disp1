#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QtCharts/QChartView>
#include<QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QTime>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>


#include "dashboard1.h"
#include "qwparameterinf.h"
#include "qwphotoes.h"
#include "torquedetection.h"
#include "setdata.h"

#include "devcomm.h"
#include "devwave.h"
#include "devoper.h"
#include "devmsgmannage.h"
#include "devreport.h"
#include "modbuspro.h"
#include "devpdcomm.h"


#define RPT_OPER_TYPE       (0x00)  // 操作报告
#define RPT_EVENT_TYPE      (0x01)  // 事件报告
#define RPT_ACT_TYPE        (0x02)  // 动作报告
#define RPT_WAVE_TYPE       (0x03)  // 录波报告

#define ADDR_TER   (0x2000)//实时温度地址
#define ADDR_HUM   (0x2001)//实时湿度地址

#define RT_WAVE_NUM         (30)

//#define MAINWND_DEBUG_EN    (true)
#if defined(MAINWND_DEBUG_EN) && MAINWND_DEBUG_EN
#define mainwnd_debug     qDebug
#else
#define mainwnd_debug(...)
#endif

using namespace QtCharts;
extern Devcomm*         g_Devcomm;
extern Devoper*         g_Devoper;
extern Devwave*         g_Devwave;
extern DevMsgMannage*   g_DevMsgMannage;
extern Devpdcomm*       g_DevpdComm;
extern Devreport*       g_Devreport;
extern ModbusPro*       g_ModbusPro;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //void dataplot_tor(uint16_t* val);
    //void dataplot_ang(uint16_t* val);
    //void dataplot_cur(uint16_t* val);

    //void dataplot_torque(int n,int m);
    //void dataplot_angel(int n,int m);
    //void dataplot_current(int n,int m);

    //从数据库中获取温度值
    int GetTemFromTab();
    //从数据库中获取湿度值
    int GetHumFromTab();
    //从录波文件中获取数据
    int GetDataFromWaveFile();

private slots:
    //温度湿度变化的槽函数
    void changeTem(int data);
    void changeHum(int data);

    void axisY_angel();
    void axisY_current();

    void m_timerSlot();

    void rtWaveTimer();

    void updateChartTor();
    void updateChartTor1();

    void updateSeriesTor(uint16_t* pVal, uint32_t idx);
    void updateSeriesAng(uint16_t* pVal, uint32_t idx);
    void updateSeriesCur(uint16_t* pVal, uint32_t idx);

    void updateChart();
signals:



private:
    //初始化界面
    void initTerminal();
    //设置窗口的外观
    void setWindowStyle();
    void sleep(unsigned int msec);
    void creatChart();//创建图表

    QChart *chartTor=new QChart;
    QChart *chartAnlCur=new QChart;

    QSplineSeries *series=new QSplineSeries(this);
    QSplineSeries *series1=new QSplineSeries(this);
    QSplineSeries *series2=new QSplineSeries(this);

    QValueAxis *axisX=new QValueAxis(this);
    QValueAxis *axisX1=new QValueAxis(this);
    QValueAxis *axisY=new QValueAxis(this);
    QValueAxis *axisY1=new QValueAxis(this);
    QValueAxis *axisY2=new QValueAxis(this);


    QTimer* m_timer=new QTimer(this);
    QTimer* m_wavetimer=new QTimer(this);

    Devwave *m_WaveFile;


private:
    Ui::MainWindow *ui;


    void updateSwState();
};
#endif // MAINWINDOW_H
