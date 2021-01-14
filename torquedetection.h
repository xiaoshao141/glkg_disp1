#ifndef TORQUEDETECTION_H
#define TORQUEDETECTION_H

#include <QDialog>
#include  "setdata.h"
#include  "comanalysis.h"
#include "devwave.h"
#include "callout.h"
#include "qchartmouseevent.h"

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include <QDateTime>
#include <QGraphicsView>

using namespace QtCharts;
namespace Ui {
class torqueDetection;
}

class torqueDetection : public QDialog
{
    Q_OBJECT

public:
    explicit torqueDetection(QWidget *parent = 0);
    ~torqueDetection();
public:
    void dataPlot_tor(uint16_t* val);
    void dataPlot_ang(uint16_t* val);
    void dataPlot_cur(uint16_t* val);

    void SaveWavedata(WaveData_t *wavedata);

private slots:

    void on_btsetdata_clicked();
    void on_btanalysis_clicked();
    void on_btpreservation_clicked();

    void axisY_angel();
    void axisY_torque();
    void axisY_current();

    void onMouseMoving(QPointF point);//鼠标移动
    void onMouseLeave();

signals:


protected:
   void wheelEvent(QWheelEvent *event);

private:
    void creatChart();//创建图表
    void torqueInit();
    void sleep(unsigned int msec);

    qreal getYValue(QPointF p1, QPointF p2, qreal x);


private:
    Ui::torqueDetection *ui;

    QSplineSeries *series1=new QSplineSeries(this);//创建数据点
    QSplineSeries *series2=new QSplineSeries(this);//创建数据点
    QSplineSeries *series3=new QSplineSeries(this);//创建数据点




public:

    QChart *chart=new QChart;
    Devwave *m_WaveFile =new Devwave;
    WaveData_t * m_wavedata = new WaveData_t;
    SetData *m_setdata=new SetData(this);//设置数据窗口
    ComAnalysis *analysis=new ComAnalysis(this);//分析数据窗口

    QValueAxis *axisX=new QValueAxis(this);
    QValueAxis *axisY=new QValueAxis(this);
    QValueAxis *axisY1=new QValueAxis(this);
    QValueAxis *axisY2=new QValueAxis(this);
    QGraphicsSimpleTextItem* m_coordItem = NULL;
    void setHintCurr();
    void setHintTor();
};

#endif // TORQUEDETECTION_H
