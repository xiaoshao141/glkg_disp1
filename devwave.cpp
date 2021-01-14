#include "devwave.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

Devwave::Devwave()
{
}

Devwave::~Devwave()
{
}

int Devwave::Init(void)
{
    int     ret = -1;
    QDir    dir;

    m_WaveFileNO = 0;
    m_WaveFileNum = 0;

    m_NowAngIndex = 0;
    m_NowCurIndex = 0;
    m_NowTorIndex = 0;
    memset(&m_RtWaveData, 0, sizeof(WaveData_t));

    m_RtWaveFlag = false;

    dir.setPath(DEV_WAVE_DIR);
    if(!dir.exists())
        dir.mkpath(DEV_WAVE_DIR);

    dir.setPath(DEV_WAVE_YDIR);
    if(!dir.exists())
        dir.mkpath(DEV_WAVE_YDIR);

    return ret;
}

int Devwave::WaveFileImport(QString fileName, WaveData_t* waveData)
{
    int         ret = -1;
    QFile       file;
    QString     filePath;
    bool        fileRet = false;

    if(fileName == "HeYangBenFile.dat" ||fileName == "FenYangBenFile.dat" )
    {
        filePath = DEV_WAVE_YDIR;
    }
    else if(fileName == "YangBenFile.dat")
    {
        filePath = DEV_WAVE_YDIR;
    }
    else
    {
        filePath = DEV_WAVE_DIR;
    }
    filePath.append(fileName);

    file.setFileName(filePath);

    fileRet = file.open(QIODevice::ReadWrite);

    if (fileRet)
    {
        file.write((const char *)waveData, sizeof(WaveData_t));
        fileRet = file.waitForBytesWritten(3000);
        ret = (int)fileRet;
        if(fileRet)
            devwave_debug("Wave File Write Successed \r\n");
        else
            devwave_debug("Wave File Write Errored \r\n");

        file.close();
    }

    return ret;
}

int Devwave::WaveFileExport(QString fileName, WaveData_t* waveData)
{
    int         ret = -1;
    QFile       file;
    QString     filePath;
    bool        fileRet = false;
    qint64      fileSize = -1;

    if(fileName == "HeYangBenFile.dat" ||fileName == "FenYangBenFile.dat")
    {
        filePath = DEV_WAVE_YDIR;
    }
    else if(fileName == "YangBenFile.dat")
    {
        filePath = DEV_WAVE_YDIR;
    }
    else
    {
        filePath = DEV_WAVE_DIR;
    }
    filePath.append(fileName);
    file.setFileName(filePath);

    fileRet = file.open(QIODevice::ReadWrite);
    if (fileRet)
    {
        fileSize = file.read((char *)waveData, sizeof(WaveData_t));
        ret = (int)fileSize;
        if(fileSize > 0)
            devwave_debug("Wave File read Successed \r\n");
        else
            devwave_debug("Wave File read Errored \r\n");

        file.close();
    }

    return ret;
}

int Devwave::WaveFileQuery(QString fileName[])
{
    QDir            dir;
    QFileInfoList   fileInfoList;


    dir.setPath(DEV_WAVE_DIR);

//    QStringList filters;
//    filters << "*.dat";
//    dir.setNameFilters(filters);
    fileInfoList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    for(int i=0; i<fileInfoList.size(); i++)
    {
        fileName[i] = fileInfoList.at(i).fileName();
    }

    return 1;
}

int Devwave::WaveFileClrNO(void)
{
    int ret = -1;

    m_WaveFileNO = 0;

    return ret;
}

int Devwave::WaveFileClear(void)
{
    int     ret = -1;
    QDir    dir;

    dir.setPath(DEV_WAVE_DIR);
    if(dir.exists())
        dir.rmdir(DEV_WAVE_DIR);

    if(!dir.exists())
        dir.mkdir(DEV_WAVE_DIR);

    ret = 1;
    return ret;
}

int Devwave::WaveFileGetNum()
{
    QFileInfoList   fileInfoList;
    QDir            dir;

    dir.setPath(DEV_WAVE_DIR);
    fileInfoList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    m_WaveFileNum = fileInfoList.size();

    return m_WaveFileNum;
}

QString Devwave::WaveFileCurrent()
{
    int currentNo = 0;

    QString fileName[DEV_WAVE_FILE_MAX];

    WaveFileQuery(fileName);

    currentNo = WaveFileGetNum() - 1;

    return fileName[currentNo];
}

void Devwave::UpdateRtWave(uint16_t type, uint16_t data)
{
    if(m_RtWaveFlag)
    {
        switch (type)
        {
            case 0:
                m_RtWaveData.Ang[m_NowAngIndex] = data;
                m_NowAngIndex++;
                if(m_NowAngIndex >= DEV_WAVE_POT)
                    m_NowAngIndex = 0;
                break;
            case 1:
                m_RtWaveData.Cur[m_NowCurIndex] = data;
                m_NowCurIndex++;
                if(m_NowCurIndex >= DEV_WAVE_POT)
                    m_NowCurIndex = 0;
                break;
            case 2:
                m_RtWaveData.Tor[m_NowTorIndex] = data;
                m_NowTorIndex++;
                if(m_NowTorIndex >= DEV_WAVE_POT)
                    m_NowTorIndex = 0;
                break;
        }
    }
}

void Devwave::getWave(WaveData_t* pData)
{
    memcpy(pData, &m_RtWaveData, sizeof(WaveData_t));
}

void Devwave::getIdx(unsigned int* torIndx, unsigned int* angIdx, unsigned int* curIdx)
{
    *torIndx = m_NowTorIndex;
    *angIdx = m_NowAngIndex;
    *curIdx = m_NowCurIndex;
}

void Devwave::startRtWave()
{
    m_NowTorIndex = 0;
    m_NowAngIndex = 0;
    m_NowCurIndex = 0;

    m_RtWaveFlag = true;
}

void Devwave::stopRtWave()
{
    m_RtWaveFlag = false;
}

bool Devwave::isStartWave()
{
    return m_RtWaveFlag;
}

bool Devwave::swState()
{
    return m_SwState;
}

void Devwave::setSwState(bool swState)
{
    m_SwState = swState;
}
