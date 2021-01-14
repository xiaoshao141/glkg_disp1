#ifndef MOUSE1_H
#define MOUSE1_H

#include "mainwindow.h"
#include <QObject>
#include <QWidget>

class mouse1 : public QWidget
{
    Q_OBJECT
public:
    explicit mouse1(QWidget *parent = nullptr);

    QWParameterInf * myparameter = new QWParameterInf(this);//温湿度信息窗口

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:


signals:

};

#endif // MOUSE1_H
