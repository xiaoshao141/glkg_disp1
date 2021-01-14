#ifndef MOUSE3_H
#define MOUSE3_H

#include "mainwindow.h"
#include <QObject>
#include <QWidget>

class mouse3 : public QWidget
{
    Q_OBJECT
public:
    explicit mouse3(QWidget *parent = nullptr);

     torqueDetection *mytorque = new torqueDetection(this);//扭矩窗口

protected:
    void mousePressEvent(QMouseEvent *event);

signals:

};

#endif // MOUSE3_H
