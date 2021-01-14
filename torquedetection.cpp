#include "torquedetection.h"
#include "ui_torquedetection.h"

#include<QTime>
#include <QDebug>
#include <QMessageBox>

torqueDetection::torqueDetection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::torqueDetection)
{
    ui->setupUi(this);

    setWindowTitle("隔离开关操作机构扭矩监测界面");    
    torqueInit();

    connect(series1,&QSplineSeries::pressed,this,&torqueDetection::axisY_angel);
    connect(series2,&QSplineSeries::pressed,this,&torqueDetection::axisY_torque);
    connect(series3,&QSplineSeries::pressed,this,&torqueDetection::axisY_current);

    connect(ui->chartviewMonitor, &QChartMouseEvent::mouseMoving, this, &torqueDetection::onMouseMoving);
    connect(ui->chartviewMonitor, &QChartMouseEvent::mouseLeave, this,  &torqueDetection::onMouseLeave);
}

torqueDetection::~torqueDetection()
{
    delete ui;
}
void torqueDetection::sleep(unsigned int msec)//延时函数（测试）
{
    QTime reachTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void torqueDetection::torqueInit()
{
    creatChart();
    chart->zoomReset();

}
void torqueDetection::creatChart()//创建图表
{   
    chart->setTitle("");
    ui->chartviewMonitor->setChart(chart);
    ui->chartviewMonitor->setRenderHint(QPainter::Antialiasing);

    series1->setName("角度");
    series1->setPointsVisible(true);

    series2->setName("扭矩");
    series2->setPointsVisible(true);

    series3->setName("电流");
    series3->setPointsVisible(true);

    //绘制横纵坐标
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(Qt::black);
    series1->setPen(pen);
    chart->addSeries(series1);
    QPen pen1;
    pen1.setStyle(Qt::SolidLine);
    pen1.setWidth(1);
    pen1.setColor(Qt::green);
    series2->setPen(pen1);
    chart->addSeries(series2);

    QPen pen2;
    pen2.setStyle(Qt::SolidLine);
    pen2.setWidth(1);
    pen2.setColor(Qt::blue);
    series3->setPen(pen2);
    chart->addSeries(series3);

//当前坐标轴  横坐标
      axisX->setRange(0, 20);//设置坐标范围
      axisX->setLabelFormat("%d");
      axisX->setTickCount(21);//主分隔个数
      axisX->setMinorTickCount(0);
      axisX->setTitleText("时间(s)");

//扭矩纵坐标
      axisY->setRange(0,400);//设置坐标范围
      axisY->setLabelFormat("%d");
      axisY->setTickCount(5);//主分隔个数
      axisY->setMinorTickCount(0);
      axisY->setTitleText("扭矩(N)");
      axisY->setLinePenColor(Qt::green);

//角度纵坐标
      axisY1->setRange(0,180);//设置坐标范围
      axisY1->setLabelFormat("%d");
      axisY1->setTickCount(19);//主分隔个数
      axisY1->setMinorTickCount(0);
      axisY1->setTitleText("角度(θ)");
      axisY1->setLabelsVisible(false);
      axisY1->setTitleVisible(false);
      axisY1->setVisible(false);
      axisY1->setLinePenColor(Qt::black);//坐标轴颜色

//电流纵坐标
      axisY2->setRange(0, 3.0);//设置坐标范围
      axisY2->setLabelFormat("%.2f");
      axisY2->setTickCount(4);//主分隔个数
      axisY2->setMinorTickCount(0);
      axisY2->setTitleText("电流(A)");
      axisY2->setLabelsVisible(false);
      axisY2->setTitleVisible(false);
      axisY2->setVisible(false);
      axisY2->setLinePenColor(Qt::blue);

      chart->setAxisX(axisX,series1);//设置X坐标
      chart->setAxisY(axisY1,series1);//设置Y坐标

      chart->addAxis(axisY, Qt::AlignLeft);
      series2->attachAxis(axisY);
      series2->attachAxis(axisX);


      chart->addAxis(axisY2, Qt::AlignLeft);
      series3->attachAxis(axisY2);
      series3->attachAxis(axisX);
}

void torqueDetection::axisY_torque()
{
    axisY1->setLabelsVisible(false);
    axisY1->setTitleVisible(false);
    axisY1->setVisible(false);
    axisY2->setLabelsVisible(false);
    axisY2->setTitleVisible(false);
    axisY2->setVisible(false);
    axisY->setLabelsVisible(true);
    axisY->setTitleVisible(true);
    axisY->setVisible(true);
}
void torqueDetection::axisY_angel()
{
    axisY->setLabelsVisible(false);
    axisY->setTitleVisible(false);
    axisY->setVisible(false);
    axisY2->setLabelsVisible(false);
    axisY2->setTitleVisible(false);
    axisY2->setVisible(false);
    axisY1->setLabelsVisible(true);
    axisY1->setTitleVisible(true);
    axisY1->setVisible(true);
}
void torqueDetection::axisY_current()
{
    axisY1->setLabelsVisible(false);
    axisY1->setTitleVisible(false);
    axisY1->setVisible(false);
    axisY->setLabelsVisible(false);
    axisY->setTitleVisible(false);
    axisY->setVisible(false);
    axisY2->setLabelsVisible(true);
    axisY2->setTitleVisible(true);
    axisY2->setVisible(true);
}

void torqueDetection::dataPlot_tor(uint16_t* val)
{
    series2->clear();

    for (int i = 0; i < 128; i++)
    {
        series2->append((20.0*i)/128, val[i*8]);
    }
}

void torqueDetection::dataPlot_ang(uint16_t* val)
{
    series1->clear();

    for (int i = 0; i < 128; i++)
    {
        series1->append((20.0*i)/128, val[i*8]);
    }
}

void torqueDetection::dataPlot_cur(uint16_t* val)
{
    series3->clear();

    for (int i = 0; i < 128; i++)
    {
        series3->append((20.0*i)/128, (qreal)(val[i*8])/10.0);
    }
}

void torqueDetection::SaveWavedata(WaveData_t *wavedata)
{
    memcpy((uint8_t *)m_wavedata, (uint8_t *)wavedata, sizeof(WaveData_t));
}

void torqueDetection::setHintCurr()
{
    axisY_torque();
    chart->removeSeries(series3);
}

void torqueDetection::setHintTor()
{
    axisY_current();
    chart->removeSeries(series2);
}

void torqueDetection::on_btsetdata_clicked()//设置参数
{
   m_setdata->setFocus();
   m_setdata->exec();
}
void torqueDetection::on_btanalysis_clicked()//数据分析窗口
{
    analysis->showMaximized();
}

void torqueDetection::on_btpreservation_clicked()//数据存储为样本
{
    QString fileNameHe,fileNameFen;
    fileNameHe = "HeYangBenFile.dat";
    fileNameFen = "FenYangBenFile.dat";
    if(m_wavedata->BrkOper == BrkOper_Off)
    {
        m_WaveFile->WaveFileImport(fileNameHe,m_wavedata);
        QMessageBox::about(this, tr("About消息框"), tr("保存为合闸样本成功！"));
    }
    else if(m_wavedata->BrkOper == BrkOper_On)
    {
        m_WaveFile->WaveFileImport(fileNameFen,m_wavedata);
        QMessageBox::about(this, tr("About消息框"), tr("保存为分闸样本成功！"));
    }


}



void torqueDetection::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
    {
        chart->zoom(1.1);   // 放大
    }
    else
    {
        chart->zoom(0.9);   // 缩小
    }
}

void torqueDetection::onMouseMoving(QPointF point)
{
    qreal pointX=point.x();
    QList<QAbstractSeries *> seriesList=chart->series();
    QString text="";
    QString text1="";
    QPointF getpointang,getpointcur;

    QLineSeries* series=(QLineSeries*)seriesList.at(0);
    QList<QPointF> points=series->points();
    for(int j=0;j<points.size();j++){
        QPointF curPoint=points.at(j) ;
        if(curPoint.x()>pointX)
        {
            if(j-1>=0){
                qreal pointY=getYValue(curPoint, points[j-1],pointX);
                QPointF point(pointX,pointY);
                getpointang=point;
                text=tr("%1").arg(point.y());
                ui->lineAngle->clear();
                ui->lineAngle->setText(text);
            }
            break;
        }
    }

    QLineSeries* series1=(QLineSeries*)seriesList.at(1);
    QList<QPointF> points1=series1->points();
    for(int j=0;j<points1.size();j++){
        QPointF curPoint1=points1.at(j) ;
        if(curPoint1.x()>pointX)
        {
            if(j-1>=0){
                qreal pointY1=getYValue(curPoint1, points1[j-1],pointX);
                QPointF point(pointX,pointY1);
                getpointcur=point;
                text1=tr("%1").arg(point.y());
                ui->lineTorqueal->clear();
                ui->lineTorqueal->setText(text1);
            }
            break;
        }
    }

    if (!m_coordItem) {
        m_coordItem = new QGraphicsSimpleTextItem(chart);
        m_coordItem->setZValue(5);
        m_coordItem->show();
 }
        QString textStr;
        textStr = QString("角度：     %1\n扭矩/电流：%2\n时间：     %3").arg(getpointang.y()).arg(getpointcur.y()).arg(getpointang.x());
        m_coordItem->setVisible(true);
        m_coordItem->setText(textStr);
        m_coordItem->setPos(chart->mapToPosition(point) + QPoint(10, -40));

}
qreal torqueDetection::getYValue(QPointF p1, QPointF p2, qreal x)
{
    qreal y=(p2.y()-p1.y())/(p2.x()-p1.x())*(x-p1.x())+p1.y();
    return y;

}

void torqueDetection::onMouseLeave()
{
    if(m_coordItem)
    {
        m_coordItem->setVisible(false);
    }
}

