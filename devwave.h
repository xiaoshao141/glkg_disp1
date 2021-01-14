#ifndef DEVWAVE_H
#define DEVWAVE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QObject>

#define DEV_WAVE_DIR                "D:\\Qt_Dir\\WaveFile\\"
#define DEV_WAVE_YDIR               "D:\\Qt_Dir\\YBFile\\"

#define DEV_WAVE_POT                (1024)
#define DEV_WAVE_BUFSIZE            (DEV_WAVE_POT*2)

#define DEV_WAVE_FILE_FORMAT        "WAVE_%02d_%04d%02d%02d_%02d%02d%02d_%03d.dat"

#define DEV_WAVE_FILE_MAX           (256)

//#define DEVWAVE_DEBUG_EN    (true)
#if defined(DEVWAVE_DEBUG_EN) && DEVWAVE_DEBUG_EN
#define devwave_debug     qDebug
#else
#define devwave_debug(...)
#endif

/*!
 * @brief 定义隔离开关分合位控制
 */
typedef enum
{
    BrkOper_On = 0,     /*!< 隔离开关　分闸 */

    BrkOper_Off         /*!< 隔离开关　合闸 */

} eBrkOper;

/*!
 * @brief 定义录波数据结构
 */
#pragma pack(push)
#pragma pack(1)
typedef struct
{
    eBrkOper    BrkOper;

    uint16_t    Tor[DEV_WAVE_POT];

    uint16_t    Ang[DEV_WAVE_POT];

    uint16_t    Cur[DEV_WAVE_POT];

} __attribute__((packed)) WaveData_t;
#pragma pack(pop)

class Devwave : public QObject
{
public:
    Devwave();
    ~Devwave();

    QString WaveFileCurrent();

    void UpdateRtWave(uint16_t type, uint16_t data);

    void getWave(WaveData_t* pData);

    void getIdx(unsigned int* torIndx, unsigned int* angIdx, unsigned int* curIdx);

    void startRtWave();
    void stopRtWave();
    bool isStartWave();
    bool swState();
    void setSwState(bool swState);
public slots:
    int Init(void);

    /* @brief 从录波文件中导入/导出录波数据 */
    int WaveFileImport(QString fileName, WaveData_t* waveData);

    int WaveFileExport(QString fileName, WaveData_t* waveData);

    int WaveFileQuery(QString fileName[]);

    int WaveFileClrNO(void);

    int WaveFileClear(void);

    int WaveFileGetNum(void);

private:
    unsigned int m_WaveFileNO;

    unsigned int m_WaveFileNum;

    unsigned int m_NowAngIndex;

    unsigned int m_NowCurIndex;

    unsigned int m_NowTorIndex;

    WaveData_t m_RtWaveData;

    bool m_RtWaveFlag;

    bool m_SwState;
};

#endif // DEVWAVE_H
