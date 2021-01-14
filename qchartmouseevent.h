#ifndef QCHARTMOUSEEVENT_H
#define QCHARTMOUSEEVENT_H

#include <QChartView>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QGestureEvent>
QT_CHARTS_USE_NAMESPACE
class QChartMouseEvent : public QChartView
{
    Q_OBJECT

public:
    QChartMouseEvent(QWidget* parent = nullptr);
    ~QChartMouseEvent();

signals:
    void mouseMoving(QPointF point);
    void mouseLeave();

protected:

    void mouseMoveEvent(QMouseEvent* event);
    void leaveEvent(QEvent* event);

private:
    bool m_isPress = false;
    double m_xMin, m_xMax, m_yMin, m_yMax = 0.0;
    QGraphicsSimpleTextItem* m_coordItem = NULL;
    QGraphicsSimpleTextItem* m_coordPointFront = NULL;
    QGraphicsSimpleTextItem* m_coordPointBack = NULL;

    QGraphicsLineItem *m_LineItem= NULL;
};


#endif // QCHARTMOUSEEVENT_H
