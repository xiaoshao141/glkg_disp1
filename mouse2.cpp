#include "mouse2.h"

#include<QMouseEvent>
#include<QDebug>

#include "mainwindow.h"

mouse2::mouse2(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);//设置追踪鼠标
}
void mouse2::mousePressEvent(QMouseEvent *event)
{
    if(g_Devwave->isStartWave()==false)
    {
        mytorque->setHintCurr();

        mytorque->showMaximized();
        mytorque->chart->zoomReset();
    }

}
