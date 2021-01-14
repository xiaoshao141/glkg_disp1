#ifndef COMANALYSIS_H
#define COMANALYSIS_H

#include <QDialog>
#include <QtCharts/QChartView>
#include<QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QTimer>

#include "devwave.h"

#define INIT_TOR_FLAG  (2)
#define INIT_CUR_FLAG  (2)
#define STAR_TOR_FLAG  (1)
#define STAR_CUR_FLAG  (1)
#define STOP_TOR_FLAG  (0)
#define STOP_CUR_FLAG  (0)

#define TOR_DATA_SETTING (0.2)
#define CUR_DATA_SETTING (0.5)



#define FILE_IMPORT_DEF_PATH        "C:\\"

using namespace QtCharts;
namespace Ui {
class ComAnalysis;
}

class ComAnalysis : public QDialog
{
    Q_OBJECT

public:
    explicit ComAnalysis(QWidget *parent = 0);
    ~ComAnalysis();

private:
    Ui::ComAnalysis *ui;

private slots:
    void changeTest(int row, int column);
    void testSlot(int row, int column);

    void on_chkCurShowTor_stateChanged(int arg1);

    void on_chkCurShowAng_stateChanged(int arg1);

    void on_chkCurShowCur_stateChanged(int arg1);

    void on_chkYangShowTor_stateChanged(int arg1);

    void on_chkYangShowAng_stateChanged(int arg1);

    void on_chkYangShowCur_stateChanged(int arg1);

    void axisY_angel();
    void axisY_torque();
    void axisY_current();

    void on_btnSaveToYB_clicked();

    void on_btnDataExport_clicked();

    void on_btnDataImport_clicked();

    void on_btnDataDelete_clicked();

    void m_timerSlot();

    void on_btnStarTorComparea_clicked();

    void on_btnStarCurComparea_clicked();

    void on_btnStopTorCompare_clicked();

    void on_btnStopCurCompare_clicked();



signals:


private:
    QSplineSeries *series1=new QSplineSeries(this);//创建数据点
    QSplineSeries *series2=new QSplineSeries(this);
    QSplineSeries *series3=new QSplineSeries(this);
    QSplineSeries *series4=new QSplineSeries(this);
    QSplineSeries *series5=new QSplineSeries(this);
    QSplineSeries *series6=new QSplineSeries(this);

    QChart *chart=new QChart;

    Devwave m_WaveFile;
    WaveData_t m_waveData;

    Devwave m_YBWaveFile;
    WaveData_t m_YBwaveData;

    QString m_filename[100];

    QStringList m_SelectFileList;

    QTimer* m_timer=new QTimer(this);

    int mLastWaveFileNum = 0;

    int rows =0;

    int starTorFlags = INIT_TOR_FLAG;
    int starCurFlags = INIT_CUR_FLAG;

    void ReadCurData(QString Filename, WaveData_t *pWaveData);
    void checkInit();
    void tablewidgetInit();
    void m_creatChart();

public:
    QValueAxis *axisX=new QValueAxis(this);
    QValueAxis *axisY1=new QValueAxis(this);
    QValueAxis *axisY2=new QValueAxis(this);
    QValueAxis *axisY3=new QValueAxis(this);

     void UpdateFileToTab();

};

#endif // COMANALYSIS_H
