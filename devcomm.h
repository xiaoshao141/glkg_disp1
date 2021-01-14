#ifndef DEVCOMM_H
#define DEVCOMM_H

#include "devmsgmannage.h"
#include "devwave.h"

#include "stdint.h"

#include <QObject>
#include <QQueue>
#include <QThread>
#include <QUdpSocket>
#include <QString>

#define DEV_HOST_IPADDR             "192.168.0.200"
#define DEV_HOST_PORT               (2000)

#define DEV_SGM0_IPADDR             "192.168.0.100"
#define DEV_SGM0_PORT               (1000)

#define DEV_SGM1_IPADDR             "192.168.0.30"
#define DEV_SGM1_PORT               (3000)

#define YX_SWOFF_ADDR               (0x1006)
#define YX_SWON_ADDR                (0x1007)
#define YX_SWWAVE_ADDR              (0x1008)

#define YC_RTTOR_ADDR               (0x2005)
#define YC_RTCUR_ADDR               (0x2006)
#define YC_RTANG_ADDR               (0x2007)

#define DEVCOMM_DEBUG_EN            (true)
#if defined(DEVCOMM_DEBUG_EN) && DEVCOMM_DEBUG_EN
#define devcomm_debug     qDebug
#else
#define devcomm_debug(...)
#endif

/* The definition is used to swap the high and low byte in 2 bytes. */
#define _SWAP_HILW_2B_(pdat)            \
    do                                  \
    {                                   \
          uint8_t tmp;                  \
          tmp = *pdat;                  \
          *pdat = *(pdat+1);            \
          *(pdat+1) = tmp;              \
    } while(0)

/* The definition is used to swap the high and low byte in 4 bytes. */
#define _SWAP_HILW_4B_(pdat)            \
    do                                  \
    {                                   \
        uint8_t temp = 0;               \
                                        \
        for (int i=0; i<=3; i++)        \
        {                               \
            temp = *(pdat+3-i);         \
            *(pdat+3-i) = *(pdat+i);    \
            *(pdat+i) = temp;           \
        }                               \
    } while(0)

class DevRecvThread: public QThread
{
public:
    DevRecvThread();
    void run();

};

class DevSendThread: public QThread
{
public:
    DevSendThread();
    void run();

};

class DevProcThread: public QThread
{
public:
    DevProcThread();
    void run();
};


class Devcomm : public QObject
{
public:
    Devcomm();
    ~Devcomm();

    int Init(void);

    bool DevMsgCheck(uint8_t* msg, uint16_t msgLen);
    uint8_t DevMsgCalcSum(uint8_t* data, uint16_t dataLen);

    int DevMsgPacket(DevMsg_t* devMsg, uint8_t* msg, uint16_t* msgLen);
    int DevMsgParse(uint8_t* msg, uint16_t msgLen, DevMsg_t* devMsg);

    int DevDataPtSQ0Packet(DevDataPtSQ0_t* devDataPt, uint8_t* data, uint16_t *dataLen);
    int DevDataPtSQ0Parse(uint8_t* data, uint16_t dataLen, DevDataPtSQ0_t* devDataPt);

    int DevDataPtSQ1Packet(DevDataPtSQ1_t* devDataPt, uint8_t* data, uint16_t* dataLen);
    int DevDataPtSQ1Parse(uint8_t* data, uint16_t dataLen, DevDataPtSQ1_t* devDataPt);

    int DevDataBlkPacket(DevDataBlk_t* devDataBlk, uint8_t* data, uint16_t* dataLen);
    int DevDataBlkParse(uint8_t* data, uint16_t dataLen, DevDataBlk_t* devDataBlk);

    int DevGenDataPtSQ0(eCtrlField ctrlCmd, DevDataPtSQ0_t* devDataPt, DevMsg_t* devMsg);
    int DevGenDataPtSQ1(eCtrlField ctrlCmd, DevDataPtSQ1_t* devDataPt, DevMsg_t* devMsg);

    int DevGenAskAll(DevMsg_t* devMsg);
    int DevGenRplyAck(DevMsg_t* devMsg);
    int DevGenRplyNAck(DevMsg_t* devMsg);

    void DevMsgProcDataPt(DevMsg_t* devMsg);
    void DevMsgProcYxChg(DevMsg_t* devMsg);
    void DevMsgProcYcChg(DevMsg_t* devMsg);
    void DevMsgProcDataBlk(DevMsg_t* devMsg);
    void DevMsgProcAck(DevMsg_t* devMsg);
    void DevMsgProcNAck(DevMsg_t* devMsg);

    void DevYxChgUpdate(uint16_t yxAddr, uint32_t yxData);
    void DevYcChgUpdate(uint16_t ycAddr, uint32_t ycData);

private:
    DevRecvThread*      m_DevRecvThread;

    DevSendThread*      m_DevSendThread;

    DevProcThread*      m_DevProcThread;
};

#endif // DEVCOMM_H
