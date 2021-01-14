#include "mouse3.h"

#include<QMouseEvent>
#include<QDebug>

#include "mainwindow.h"

mouse3::mouse3(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);//设置追踪鼠标
}
void mouse3::mousePressEvent(QMouseEvent *event)
{
    if(g_Devwave->isStartWave()==false)
    {
        mytorque->setHintTor();

        mytorque->showMaximized();
        mytorque->chart->zoomReset();
    }

}
