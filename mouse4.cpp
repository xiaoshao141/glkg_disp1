#include "mouse4.h"

#include<QMouseEvent>
#include<QDebug>

mouse4::mouse4(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);//设置追踪鼠标
}
void mouse4::mousePressEvent(QMouseEvent *event)
{

 //   myphotoes->setWindowState(Qt::WindowMaximized);
    myphotoes->showMaximized();
}
