#ifndef DEVPDCOMM_H
#define DEVPDCOMM_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork>
#include <QThread>

#define DEVPD_TCP_IPADDR            "10.1.0.3"
#define DEVPD_TCP_PORT              5130

#define DEVPD_UHF_OBJADDR           (8203)  /*!< 局放超高频 */
#define DEVPD_AE_OBJADDR            (8204)  /*!< 局放超声波 */
#define DEVPD_TEV_OBJADDR           (8205)  /*!< 局放地电波 */

#define DEVPD_UPDATE_PERIOD         (1000)  /*!< 1000 ms */

//#define DEVPDCOMM_DEBUG_EN          (true)
#if defined(DEVPDCOMM_DEBUG_EN) && DEVPDCOMM_DEBUG_EN
#define devpdcomm_debug     qDebug
#else
#define devpdcomm_debug(...)
#endif


typedef struct _RST
{
    char nType;         // 请求数据类型， 0-局放DB值，1-PRPD, 2-PRPS
    char nChanIdx;      // 通道序号(0-2，对应通道1-3), 数据类型为1,2时有效，为0时将同时上传三通道的当前测量数据
} __attribute__((packed)) RST, *PRST;

typedef struct _PEAK_RSP
{
    float fPeak[4];     // 3个通道的局放DB值
    int  nPhase[4];     // 3个通道局放DB值对应的相位值
} __attribute__((packed)) PEAK_RSP, *PPEAK_RSP;

typedef struct _DAT_HEADER
{
    int nFileLenth;         // 数据长度
    float fVersion;         // 规范版本号
    char nType;             // 图谱类型编码0-PRPD 1-PRPS
    int nPhaseDiv;          // 相位窗数
    int nAmpDiv;            // 量化幅度
    char nAmpUnit;          // 幅值单位 1-dBm 2-mV 3-%
    float fAmpDownLimit;    // 幅值下限
    float fAmpUpLimit;      // 幅值上限
    int nPeriods;           // 工频周期数
    char Conclusion[8];     // 放电类型概率
} __attribute__((packed)) DAT_HEADER, *PDAT_HEADER;


class DevpdUpdtThread: public QThread
{
    Q_OBJECT
public:
    DevpdUpdtThread();
    void run();

private slots:
    void onReadMsg(void);
    void onConnect(void);
    void onDisConnect(void);
};

class Devpdcomm : public QObject
{
    Q_OBJECT
public:
    explicit Devpdcomm(QObject *parent = 0);
    ~Devpdcomm();

    void Init(void);

signals:
    
public slots:
    
private:
    DevpdUpdtThread*    m_DevpdUpdtThread;
};

#endif // DEVPDCOMM_H
