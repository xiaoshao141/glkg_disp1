#include "qchartmouseevent.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QValueAxis>
#include <QPainter>
QChartMouseEvent::QChartMouseEvent(QWidget* parent) : QChartView(parent) {}

QChartMouseEvent::~QChartMouseEvent() {}

void QChartMouseEvent::mouseMoveEvent(QMouseEvent* event)
{

    QChartView::mouseMoveEvent(event);
       //绘制光标
       if (! m_LineItem) {

           m_LineItem = new QGraphicsLineItem(this->chart());
           QPen pen=m_LineItem->pen();
           pen.setColor(QColor(54,121,204,100));
           pen.setWidth(3);
           m_LineItem->setPen(pen);
           this->scene()->addItem(m_LineItem);
           m_LineItem->show();
   }
       m_LineItem->setVisible(true);
       const QPointF curPos = event->pos();//const
       QPointF curVal = this->chart()->mapToValue(curPos);
       m_LineItem->setLine(QLineF(curPos.x(), 77,curPos.x(),this->rect().height()-70));
       emit mouseMoving(curVal);
}



void QChartMouseEvent::leaveEvent(QEvent*)
{

    if(m_LineItem)
    {
      m_LineItem->setVisible(false);
    }
    emit mouseLeave();

}
