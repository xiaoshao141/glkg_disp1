#include "mouse1.h"

#include<QMouseEvent>
#include<QDebug>
/*
 * mouse1,2,3，4=====》设置当鼠标点击四个小窗口时打开大窗口
*/
mouse1::mouse1(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);//设置追踪鼠标
}
void mouse1::mousePressEvent(QMouseEvent *event)
{

 //   myparameter->setWindowState(Qt::WindowMaximized);
    myparameter->showMaximized();
}
