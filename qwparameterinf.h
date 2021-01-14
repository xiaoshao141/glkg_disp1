#ifndef QWPARAMETERINF_H
#define QWPARAMETERINF_H

#include <QDialog>
#include <QPainter>
#include "qwparametersave.h"
#include "qwanalysis.h"
#include "dashboard1.h"
#include<QTime>


class MainWindow;

namespace Ui {
class QWParameterInf;
}

class QWParameterInf : public QDialog
{
    Q_OBJECT

public:
    explicit QWParameterInf(QWidget *parent = nullptr);
    ~QWParameterInf();

    //创建参数设置窗口
    QWParameterSave *mySetting = new QWParameterSave(this);
    //创建综合分析窗口
    QWAnalysis *myanalysis = new QWAnalysis(this);

private slots:
    void on_btnParameterSetting_clicked();

    void on_btnAnalsis_clicked();

    void Compare();

public:
   void sleep(unsigned int msec);
   //温度数据同步
   void TemData(int data);
   void HumData(int data);


private:
    Ui::QWParameterInf *ui;

    MainWindow *m_parent;              // 主窗口类的对象
};

#endif // QWPARAMETERINF_H
