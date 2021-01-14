#ifndef MOUSE4_H
#define MOUSE4_H

#include "mainwindow.h"
#include <QObject>
#include <QWidget>

class mouse4 : public QWidget
{
    Q_OBJECT
public:
    explicit mouse4(QWidget *parent = nullptr);

    QWPhotoes * myphotoes = new QWPhotoes(this);//红外图像窗口

protected:
    void mousePressEvent(QMouseEvent *event);

signals:

};

#endif // MOUSE4_H
