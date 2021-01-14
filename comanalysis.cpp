#include "comanalysis.h"
#include "ui_comanalysis.h"

#include "mainwindow.h"

#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QIODevice>
#include <QFileDialog>
#include <QAbstractItemView>


ComAnalysis::ComAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComAnalysis)
{
    ui->setupUi(this);
    setWindowTitle("隔离开关操作机构扭矩监测综合分析");

    connect(ui->tableWidgetWaveFile, SIGNAL(cellChanged(int,int)), this, SLOT(changeTest(int, int)));
    mLastWaveFileNum = 0;
    m_SelectFileList.clear();

    m_creatChart();
    UpdateFileToTab();
//    checkInit();
    tablewidgetInit();

    connect(series1,&QSplineSeries::pressed,this,&ComAnalysis::axisY_torque);
    connect(series2,&QSplineSeries::pressed,this,&ComAnalysis::axisY_angel);
    connect(series3,&QSplineSeries::pressed,this,&ComAnalysis::axisY_current);

    connect(series4,&QSplineSeries::pressed,this,&ComAnalysis::axisY_torque);
    connect(series5,&QSplineSeries::pressed,this,&ComAnalysis::axisY_angel);
    connect(series6,&QSplineSeries::pressed,this,&ComAnalysis::axisY_current);


//    connect(ui->tableWidgetWaveFile,SIGNAL(cellClicked(int,int)),this,SLOT(testSlot(int,int)));

    connect(m_timer,SIGNAL(timeout()), this, SLOT(m_timerSlot()));
    m_timer->start(1000);
}

ComAnalysis::~ComAnalysis()
{
    delete ui;
}

void ComAnalysis::tablewidgetInit()
{
    ui->tableWidgetWaveFile->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidgetWaveFile->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidgetWaveFile->item(0,3)->setCheckState(Qt::Checked);



    ui->chkYangShowAng->setCheckState(Qt::Checked);
    ui->chkYangShowTor->setCheckState(Qt::Checked);
    ui->chkCurShowAng ->setCheckState(Qt::Checked);
    ui->chkCurShowTor ->setCheckState(Qt::Checked);

    ui->tableWidgetWaveFile->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetWaveFile->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString Filename;
    QString FilenameYB;
    Filename = ui->tableWidgetWaveFile->item(0,1)->text();
    ReadCurData(Filename, &m_waveData);
    if(m_waveData.BrkOper == BrkOper_Off)
        FilenameYB = "HeYangBenFile.dat";
    else if(m_waveData.BrkOper == BrkOper_On)
        FilenameYB = "FenYangBenFile.dat";
        ReadCurData(FilenameYB,&m_YBwaveData);

}

void ComAnalysis::checkInit()
{
    ui->chkCurShowAng->setEnabled(false);
    ui->chkCurShowTor->setEnabled(false);
    ui->chkCurShowCur->setEnabled(false);
    ui->chkYangShowAng->setEnabled(false);
    ui->chkYangShowTor->setEnabled(false);
    ui->chkYangShowCur->setEnabled(false);
}

void ComAnalysis::m_creatChart()
{
    chart->setTitle("");
    ui->chartViewAnalysis->setChart(chart);
    ui->chartViewAnalysis->setRenderHint(QPainter::Antialiasing);

    series1->setName("扭矩");
    series1->setPointsVisible(true);

    series2->setName("角度");
    series2->setPointsVisible(true);

    series3->setName("电流");
    series3->setPointsVisible(true);

    series4->setName("样本扭矩");
    series4->setPointsVisible(true);

    series5->setName("样本角度");
    series5->setPointsVisible(true);

    series6->setName("样本电流");
    series6->setPointsVisible(true);

    //绘制横纵坐标
    QPen pen1;
    pen1.setStyle(Qt::SolidLine);
    pen1.setWidth(1);
    pen1.setColor(Qt::green);
    series1->setPen(pen1);
    chart->addSeries(series1);

    QPen pen2;
    pen2.setStyle(Qt::SolidLine);
    pen2.setWidth(1);
    pen2.setColor(Qt::black);
    series2->setPen(pen2);
    chart->addSeries(series2);

    QPen pen3;
    pen3.setStyle(Qt::SolidLine);
    pen3.setWidth(1);
    pen3.setColor(Qt::blue);
    series3->setPen(pen3);
    chart->addSeries(series3);

    QPen pen4;
    pen4.setStyle(Qt::DashLine);
    pen4.setWidth(1);
    pen4.setColor(Qt::green);
    series4->setPen(pen4);
    chart->addSeries(series4);

    QPen pen5;
    pen5.setStyle(Qt::DashLine);
    pen5.setWidth(1);
    pen5.setColor(Qt::black);
    series5->setPen(pen5);
    chart->addSeries(series5);

    QPen pen6;
    pen6.setStyle(Qt::DashLine);
    pen6.setWidth(1);
    pen6.setColor(Qt::blue);
    series6->setPen(pen6);
    chart->addSeries(series6);

//当前坐标轴  横坐标
      axisX->setRange(0,20);//设置坐标范围
      axisX->setLabelFormat("%d");
      axisX->setTickCount(21);//主分隔个数
      axisX->setMinorTickCount(0);
      axisX->setTitleText("时间(s)");

//扭矩纵坐标
      axisY1->setRange(0,400);//设置坐标范围
      axisY1->setLabelFormat("%d");
      axisY1->setTickCount(5);//主分隔个数
      axisY1->setMinorTickCount(0);
      axisY1->setTitleText("扭矩(N)");
      axisY1->setLinePenColor(Qt::green);

//角度纵坐标
      axisY2->setRange(0,180);//设置坐标范围
      axisY2->setLabelFormat("%d");
      axisY2->setTickCount(19);//主分隔个数
      axisY2->setMinorTickCount(0);
      axisY2->setTitleText("角度(θ)");
      axisY2->setLabelsVisible(false);
      axisY2->setTitleVisible(false);
      axisY2->setVisible(false);
      axisY2->setLinePenColor(Qt::black);//坐标轴颜色

//电流纵坐标
      axisY3->setRange(0,3.0);//设置坐标范围
      axisY3->setLabelFormat("%.2f");
      axisY3->setTickCount(4);//主分隔个数
      axisY3->setMinorTickCount(0);
      axisY3->setTitleText("电流(A)");
      axisY3->setLabelsVisible(false);
      axisY3->setTitleVisible(false);
      axisY3->setVisible(false);
      axisY3->setLinePenColor(Qt::blue);

      chart->setAxisX(axisX,series1);//设置X坐标
      chart->setAxisY(axisY1,series1);//设置Y坐标

      chart->setAxisX(axisX,series2);
      chart->setAxisY(axisY2,series2);

      chart->setAxisX(axisX,series3);
      chart->setAxisY(axisY3,series3);


      chart->setAxisX(axisX,series4);//设置X坐标
      chart->setAxisY(axisY1,series4);//设置Y坐标

      chart->setAxisX(axisX,series5);
      chart->setAxisY(axisY2,series5);

      chart->setAxisX(axisX,series6);
      chart->setAxisY(axisY3,series6);
}

void ComAnalysis::axisY_torque()
{
    axisY1->setLabelsVisible(true);
    axisY1->setTitleVisible(true);
    axisY1->setVisible(true);
    axisY2->setLabelsVisible(false);
    axisY2->setTitleVisible(false);
    axisY2->setVisible(false);
    axisY3->setLabelsVisible(false);
    axisY3->setTitleVisible(false);
    axisY3->setVisible(false);
}
void ComAnalysis::axisY_angel()
{
   axisY2->setLabelsVisible(true);//量程可见性
   axisY2->setTitleVisible(true);//坐标轴可见性
   axisY2->setVisible(true);//坐标轴可见性
   axisY1->setLabelsVisible(false);
   axisY1->setTitleVisible(false);
   axisY1->setVisible(false);
   axisY3->setLabelsVisible(false);
   axisY3->setTitleVisible(false);
   axisY3->setVisible(false);

}
void ComAnalysis::axisY_current()
{
    axisY3->setLabelsVisible(true);
    axisY3->setTitleVisible(true);
    axisY3->setVisible(true);
    axisY1->setLabelsVisible(false);
    axisY1->setTitleVisible(false);
    axisY1->setVisible(false);
    axisY2->setLabelsVisible(false);
    axisY2->setTitleVisible(false);
    axisY2->setVisible(false);
}

//当tablewidget中的复选框状态发射变化时，运行该槽函数
void ComAnalysis::changeTest(int row, int column)
{
    if(starTorFlags != STAR_TOR_FLAG && starCurFlags != STAR_CUR_FLAG)
    {
        if(ui->tableWidgetWaveFile->item(row, column)->checkState()==Qt::Checked)
        {
            QString Filename;
            Filename = ui->tableWidgetWaveFile->item(row,column-2)->text();
            if(!m_SelectFileList.contains(Filename))
            {
                m_SelectFileList.append(Filename);


            }

        }
        else if(ui->tableWidgetWaveFile->item(row, column)->checkState()==Qt::Unchecked)
        {

            series1->clear();
            series2->clear();
            series3->clear();

            QString Filename;
            Filename = ui->tableWidgetWaveFile->item(row,column-2)->text();
            if(m_SelectFileList.contains(Filename))
            {
                m_SelectFileList.removeOne(Filename);

            }


        }
        if(m_SelectFileList.count()!=0)
        {
            QString FilenameCur;
            QString FilenameYB;
            FilenameCur = m_SelectFileList.at(m_SelectFileList.count()-1);
            ReadCurData(FilenameCur, &m_waveData);

            if(m_waveData.BrkOper == BrkOper_Off)
                FilenameYB = "HeYangBenFile.dat";
            else if(m_waveData.BrkOper == BrkOper_On)
                FilenameYB = "FenYangBenFile.dat";
                ReadCurData(FilenameYB,&m_YBwaveData);


        }
        if(m_SelectFileList.size()==1)
        {
            ui->chkCurShowAng->setEnabled(true);
            ui->chkCurShowTor->setEnabled(true);
            ui->chkCurShowCur->setEnabled(true);
            ui->chkYangShowAng->setEnabled(true);
            ui->chkYangShowTor->setEnabled(true);
            ui->chkYangShowCur->setEnabled(true);

        }
        else{
            ui->chkCurShowAng->setEnabled(false);
            ui->chkCurShowTor->setEnabled(false);
            ui->chkCurShowCur->setEnabled(false);
            ui->chkYangShowAng->setEnabled(false);
            ui->chkYangShowTor->setEnabled(false);
            ui->chkYangShowCur->setEnabled(false);

            ui->chkCurShowAng->setCheckState(Qt::Unchecked);
            ui->chkCurShowTor->setCheckState(Qt::Unchecked);
            ui->chkCurShowCur->setCheckState(Qt::Unchecked);
            ui->chkYangShowAng->setCheckState(Qt::Unchecked);
            ui->chkYangShowTor->setCheckState(Qt::Unchecked);
            ui->chkYangShowCur->setCheckState(Qt::Unchecked);

        }

    }

}

void ComAnalysis::testSlot(int row, int column)
{
    if(ui->tableWidgetWaveFile->item(row,3)->checkState()==Qt::Unchecked)
        ui->tableWidgetWaveFile->item(row,3)->setCheckState(Qt::Checked);
    else if(ui->tableWidgetWaveFile->item(row,3)->checkState()==Qt::Checked)
        ui->tableWidgetWaveFile->item(row,3)->setCheckState(Qt::Unchecked);
}

/*
 * 当前波形的显示：
 * ReadCurData===>读取文件中的波形数据
 * 扭矩叠加chkCurShowTor
 * 角度叠加
 * 电流叠加
*/
void ComAnalysis::ReadCurData(QString Filename, WaveData_t* pWaveData)
{
    if(Filename == "FenYangBenFile.dat" || Filename =="HeYangBenFile.dat")
    {
        m_YBWaveFile.WaveFileExport(Filename,pWaveData);
    }
    else
    {
       m_WaveFile.WaveFileExport(Filename,pWaveData);
    }
}



void ComAnalysis::on_chkCurShowTor_stateChanged(int arg1)
{
    series1->clear();
    if(ui->chkCurShowTor->checkState()==Qt::Checked){
         for(int i=0;i<128;i++){
             qreal tor;
             tor = m_waveData.Tor[i*8]/1.0;
             series1->append((20.0*i)/128,tor);
          }
    }
    else if(ui->chkCurShowTor->checkState()==Qt::Unchecked){
        series1->clear();
    }


    QPen pen;
    pen = series1->pen();
    pen.setWidth(1);
    series1->setPen(pen);

    axisY_torque();
}

void ComAnalysis::on_chkCurShowAng_stateChanged(int arg1)
{
    series2->clear();
    if(ui->chkCurShowAng->checkState()==Qt::Checked){
         for(int i=0;i<128;i++){
             qreal ang;
             ang = m_waveData.Ang[i*8]/1.0;
             series2->append((20.0*i)/128,ang);
          }
    }
    else if(ui->chkCurShowAng->checkState()==Qt::Unchecked){
        series2->clear();
    }

    QPen pen;
    pen = series2->pen();
    pen.setWidth(1);
    series2->setPen(pen);

    axisY_angel();
}

void ComAnalysis::on_chkCurShowCur_stateChanged(int arg1)
{
    series3->clear();
    if(ui->chkCurShowCur->checkState()==Qt::Checked){
         for(int i=0;i<128;i++){
             qreal cur;
             cur = m_waveData.Cur[i*8]/10.0;
             series3->append((20.0*i)/128,cur);
          }
    }
    else if(ui->chkCurShowCur->checkState()==Qt::Unchecked){
        series3->clear();
    }



    QPen pen;
    pen = series3->pen();
    pen.setWidth(1);
    series3->setPen(pen);

    axisY_current();
}


void ComAnalysis::on_chkYangShowTor_stateChanged(int arg1)
{
    series4->clear();
    if(ui->chkYangShowTor->checkState()==Qt::Checked)
    {
         for(int i=0;i<128;i++){
             qreal tor;
             tor = m_YBwaveData.Tor[i*8]/1.0;
             series4->append((20.0*i)/128,tor);
         }

    }
    else if(ui->chkYangShowTor->checkState()==Qt::Unchecked){
             series4->clear();
         }

    QPen pen;
    pen = series4->pen();
    pen.setWidth(1);
    series4->setPen(pen);

    axisY_torque();
}

void ComAnalysis::on_chkYangShowAng_stateChanged(int arg1)
{
    series5->clear();
    if(ui->chkYangShowAng->checkState()==Qt::Checked){
         for(int i=0;i<128;i++){
             qreal ang;
             ang = m_YBwaveData.Ang[i*8]/1.0;
             series5->append((20.0*i)/128,ang);
          }
    }
    else if(ui->chkYangShowAng->checkState()==Qt::Unchecked){
        series5->clear();
    }

    QPen pen;
    pen = series5->pen();
    pen.setWidth(1);
    series5->setPen(pen);

    axisY_angel();
}

void ComAnalysis::on_chkYangShowCur_stateChanged(int arg1)
{
    series6->clear();
    if(ui->chkYangShowCur->checkState()==Qt::Checked){
         for(int i=0;i<128;i++){
             qreal cur;
             cur = m_YBwaveData.Cur[i*8]/10.0;
             series6->append((20.0*i)/128,cur);
          }
    }
    else if(ui->chkYangShowCur->checkState()==Qt::Unchecked){
        series6->clear();
    }


    QPen pen;
    pen = series6->pen();
    pen.setWidth(1);
    series6->setPen(pen);

    axisY_current();
}


void ComAnalysis::on_btnSaveToYB_clicked()
{
    QString fileNameHe,fileNameFen;
    fileNameHe = "HeYangBenFile.dat";
    fileNameFen = "FenYangBenFile.dat";
    if(m_waveData.BrkOper == BrkOper_Off)
    {
        m_WaveFile.WaveFileImport(fileNameHe,&m_waveData);
        QMessageBox::about(this, tr("About消息框"), tr("保存为合闸样本成功！"));
    }
    else if(m_waveData.BrkOper == BrkOper_On)
    {
        m_WaveFile.WaveFileImport(fileNameFen,&m_waveData);
        QMessageBox::about(this, tr("About消息框"), tr("保存为分闸样本成功！"));
    }
}

void ComAnalysis::on_btnDataExport_clicked()
{
    QString dir_name = QFileDialog::getExistingDirectory(NULL,"选择数据导出文件目录",".");
    if(!dir_name.isEmpty())
    {
        for(int i = 0; i< m_SelectFileList.size();++i)
        {
            QString filePath = DEV_WAVE_DIR;
            QString fileName = m_SelectFileList.at(i);
            QString newFilePath = dir_name + "\/";

            filePath.append(fileName);
            newFilePath.append(fileName);

            QFile::copy(filePath, newFilePath);
        }

    }
}

void ComAnalysis::on_btnDataImport_clicked()
{

    QStringList filePathList = QFileDialog::getOpenFileNames(this, tr("选择数据导入文件"), FILE_IMPORT_DEF_PATH, tr("*.dat"));

    QString m_newFilePath;
    for(int i = 0; i< filePathList.size();++i)
    {
        QString filePath = filePathList.at(i);
        QString newFilePath = DEV_WAVE_DIR;

        QFileInfo fileInfo = QFileInfo(filePath);
        QString fileName = fileInfo.fileName();
        newFilePath.append(fileName);
        m_newFilePath = newFilePath;
        QFile::copy(filePath, newFilePath);
    }

    ui->chkCurShowTor->setCheckState(Qt::Unchecked);
    ui->chkYangShowTor->setCheckState(Qt::Unchecked);

    ui->chkCurShowAng->setCheckState(Qt::Unchecked);
    ui->chkYangShowAng->setCheckState(Qt::Unchecked);

    ui->chkCurShowCur->setCheckState(Qt::Unchecked);
    ui->chkYangShowCur->setCheckState(Qt::Unchecked);

    series1->clear();
    series2->clear();
    series3->clear();
    series4->clear();
    series5->clear();
    series6->clear();

    ui->chkCurShowAng->setEnabled(false);
    ui->chkCurShowTor->setEnabled(false);
    ui->chkCurShowCur->setEnabled(false);
    ui->chkYangShowAng->setEnabled(false);
    ui->chkYangShowTor->setEnabled(false);
    ui->chkYangShowCur->setEnabled(false);
}

void ComAnalysis::on_btnDataDelete_clicked()
{

    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确实要删除吗?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        for(int i = 0; i< m_SelectFileList.size();++i)
        {
            QString filePath = DEV_WAVE_DIR;
            QString fileName = m_SelectFileList.at(i);

            filePath.append(fileName);
            QFile file(filePath);
            if(file.exists())
                file.remove();
        }

        m_SelectFileList.clear();


        ui->chkCurShowTor->setCheckState(Qt::Unchecked);
        ui->chkYangShowTor->setCheckState(Qt::Unchecked);

        ui->chkCurShowAng->setCheckState(Qt::Unchecked);
        ui->chkYangShowAng->setCheckState(Qt::Unchecked);

        ui->chkCurShowCur->setCheckState(Qt::Unchecked);
        ui->chkYangShowCur->setCheckState(Qt::Unchecked);

        series1->clear();
        series2->clear();
        series3->clear();
        series4->clear();
        series5->clear();
        series6->clear();

    }



}

void ComAnalysis::m_timerSlot()
{
    UpdateFileToTab();
}

void ComAnalysis::UpdateFileToTab()
{
    WaveData_t fileData;

    int num = m_WaveFile.WaveFileGetNum();
    QTableWidgetItem *item = new QTableWidgetItem();

    if(num != mLastWaveFileNum)
    {
        mLastWaveFileNum = num;
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);

        disconnect(ui->tableWidgetWaveFile, SIGNAL(cellChanged(int,int)), this, SLOT(changeTest(int, int)));
        disconnect(ui->tableWidgetWaveFile,SIGNAL(cellClicked(int,int)),this,SLOT(testSlot(int,int)));

        m_WaveFile.WaveFileQuery(m_filename);
        ui->tableWidgetWaveFile->setRowCount(num);
        ui->tableWidgetWaveFile->insertRow(num);
        int m_number = num;

        ui->tableWidgetWaveFile->setItem(0,0,new QTableWidgetItem(1));
        for(int i=0;i<num;i++)
        {
            ui->tableWidgetWaveFile->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));

            m_number--;
            ui->tableWidgetWaveFile->setItem(i,1,new QTableWidgetItem(m_filename[m_number]));
            QTableWidgetItem *check=new QTableWidgetItem;
            check->setCheckState (Qt::Unchecked);
            ui->tableWidgetWaveFile->setItem(i,3,check); //插入复选框

            ui->tableWidgetWaveFile->setItem(i,0,item);
            ui->tableWidgetWaveFile->setItem(i,1,item);
            ui->tableWidgetWaveFile->setItem(i,2,item);

            m_WaveFile.WaveFileExport(m_filename[m_number],&fileData);
            if(fileData.BrkOper == BrkOper_Off)
                ui->tableWidgetWaveFile->setItem(i,2,new QTableWidgetItem("合闸"));
            else if(fileData.BrkOper == BrkOper_On)
                ui->tableWidgetWaveFile->setItem(i,2,new QTableWidgetItem("分闸"));


        }


        ui->tableWidgetWaveFile->setColumnHidden(0, true);

        connect(ui->tableWidgetWaveFile, SIGNAL(cellChanged(int,int)), this, SLOT(changeTest(int, int)));
        connect(ui->tableWidgetWaveFile,SIGNAL(cellClicked(int,int)),this,SLOT(testSlot(int,int)));


        m_SelectFileList.clear();
        tablewidgetInit();
    }

    if(starTorFlags == STAR_TOR_FLAG || starCurFlags == STAR_CUR_FLAG)
        ui->tableWidgetWaveFile->setEnabled(false);
    else if(starTorFlags == STOP_TOR_FLAG && starCurFlags == STOP_CUR_FLAG)
        ui->tableWidgetWaveFile->setEnabled(true);
}

void ComAnalysis::on_btnStarTorComparea_clicked()
{
        if(m_SelectFileList.size()!=1)
        {
            QMessageBox::information(NULL, "提示", "请先选择要一个进行比对的波形！",
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        else if(m_SelectFileList.size()==1)
        {

            ui->chkCurShowTor->setCheckState(Qt::Checked);
            ui->chkYangShowTor->setCheckState(Qt::Checked);

            ui->chkCurShowAng->setCheckState(Qt::Checked);
            ui->chkYangShowAng->setCheckState(Qt::Checked);

            ui->chkCurShowCur->setCheckState(Qt::Unchecked);
            ui->chkYangShowCur->setCheckState(Qt::Unchecked);


            ui->chkCurShowCur->setEnabled(false);
            ui->chkYangShowCur->setEnabled(false);

            ui->btnStarTorComparea->setDisabled(true);
            ui->btnStarCurComparea->setDisabled(true);
            ui->btnStopCurCompare->setDisabled(true);


            starTorFlags = STAR_TOR_FLAG;

            for(int i=0;i<128;i++)
            {
                if(m_waveData.Tor[i*8] >= 50 && m_YBwaveData.Tor[i*8] >= 50)
                {
                    if(( fabsf(m_waveData.Tor[i*8] - m_YBwaveData.Tor[i*8])/m_YBwaveData.Tor[i*8])<TOR_DATA_SETTING)
                    {
                        ui->textEditConclusion->clear();
                        ui->textEditConclusion->setTextColor(Qt::green);
                        ui->textEditConclusion->setText("开关扭矩监测正常");
                    }
                    else
                    {
                        ui->textEditConclusion->clear();
                        ui->textEditConclusion->setTextColor(Qt::red);
                        ui->textEditConclusion->setText("开关扭矩监测异常超值报警，因转动部分卡滞导致开关操作扭矩增大，建议检查调试");
                    }
                }
            }
            ui->tableWidgetWaveFile->setSelectionMode(QAbstractItemView::NoSelection);
        }     

    else
    {
    QMessageBox::information(NULL, "提示", "正在进行电流比对！请先结束电流比对！",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

}

void ComAnalysis::on_btnStarCurComparea_clicked()
{
        if(m_SelectFileList.size()!=1)
        {
            QMessageBox::information(NULL, "提示", "请先选择要一个进行比对的波形！",
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        else if(m_SelectFileList.size()==1)
        {
            ui->chkCurShowTor->setCheckState(Qt::Unchecked);
            ui->chkYangShowTor->setCheckState(Qt::Unchecked);
            ui->chkCurShowTor->setEnabled(false);
            ui->chkYangShowTor->setEnabled(false);

            ui->chkCurShowAng->setCheckState(Qt::Checked);
            ui->chkYangShowAng->setCheckState(Qt::Checked);

            ui->chkCurShowCur->setCheckState(Qt::Checked); 
            ui->chkYangShowCur->setCheckState(Qt::Checked);

            ui->btnStarTorComparea->setDisabled(true);
            ui->btnStarCurComparea->setDisabled(true);
            ui->btnStopTorCompare->setDisabled(true);


            starCurFlags=STAR_CUR_FLAG;

            for(int i=0;i<128;i++)
            {
                if(m_waveData.Cur[i*8] != 0 && m_YBwaveData.Cur[i*8] != 0)
                {
                    if(( fabsf(m_waveData.Cur[i*8] - m_YBwaveData.Cur[i*8])/m_YBwaveData.Cur[i*8])<CUR_DATA_SETTING)
                    {
                        ui->textEditConclusion->clear();
                        ui->textEditConclusion->setTextColor(Qt::green);
                        ui->textEditConclusion->setText("开关电流监测正常");
                    }
                    else
                    {
                        ui->textEditConclusion->clear();
                        ui->textEditConclusion->setTextColor(Qt::red);
                        ui->textEditConclusion->setText("开关电流监测异常超值报警");
                    }
                }
            }
            ui->tableWidgetWaveFile->setSelectionMode(QAbstractItemView::NoSelection);

        }

    else
    {
    QMessageBox::information(NULL, "提示", "正在进行扭矩比对！请先结束扭矩比对！",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

}

void ComAnalysis::on_btnStopTorCompare_clicked()
{
    if(starTorFlags==STAR_TOR_FLAG)
    {
        ui->chkCurShowTor->setCheckState(Qt::Unchecked);
        ui->chkYangShowTor->setCheckState(Qt::Unchecked);

        ui->chkCurShowAng->setCheckState(Qt::Unchecked);
        ui->chkYangShowAng->setCheckState(Qt::Unchecked);

        ui->chkCurShowCur->setCheckState(Qt::Unchecked);
        ui->chkYangShowCur->setCheckState(Qt::Unchecked);

        ui->chkCurShowCur->setEnabled(true);
        ui->chkYangShowCur->setEnabled(true);

        ui->btnStarTorComparea->setEnabled(true);
        ui->btnStarCurComparea->setEnabled(true);
        ui->btnStopCurCompare->setEnabled(true);

        starTorFlags = STOP_TOR_FLAG;
        starCurFlags = STOP_CUR_FLAG;


    }
    else
    {
      QMessageBox::information(NULL, "提示", "扭矩比对未开始！",
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    ui->textEditConclusion->clear();
    ui->tableWidgetWaveFile->setSelectionMode(QAbstractItemView::ExtendedSelection);

}

void ComAnalysis::on_btnStopCurCompare_clicked()
{
    if(starCurFlags==STAR_CUR_FLAG)
    {
        ui->chkCurShowTor->setCheckState(Qt::Unchecked);
        ui->chkYangShowTor->setCheckState(Qt::Unchecked);

        ui->chkCurShowAng->setCheckState(Qt::Unchecked);
        ui->chkYangShowAng->setCheckState(Qt::Unchecked);

        ui->chkCurShowCur->setCheckState(Qt::Unchecked);
        ui->chkYangShowCur->setCheckState(Qt::Unchecked);

        ui->chkCurShowTor->setEnabled(true);
        ui->chkYangShowTor->setEnabled(true);

        ui->btnStarTorComparea->setEnabled(true);
        ui->btnStarCurComparea->setEnabled(true);
        ui->btnStopTorCompare->setEnabled(true);

        starTorFlags = STOP_TOR_FLAG;
        starCurFlags = STOP_CUR_FLAG;

    }
    else
    {
      QMessageBox::information(NULL, "提示", "电流比对未开始！",
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    ui->textEditConclusion->clear();
    ui->tableWidgetWaveFile->setSelectionMode(QAbstractItemView::ExtendedSelection);
}


