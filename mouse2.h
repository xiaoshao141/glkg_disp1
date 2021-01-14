#ifndef MOUSE2_H
#define MOUSE2_H

#include "mainwindow.h"
#include <QObject>
#include <QWidget>

class mouse2 : public QWidget
{
    Q_OBJECT
public:
    explicit mouse2(QWidget *parent = nullptr);

     torqueDetection *mytorque = new torqueDetection(this);//扭矩窗口

protected:
    void mousePressEvent(QMouseEvent *event);

signals:

};

#endif // MOUSE2_H
