#ifndef DEVMSGMANNAGE_H
#define DEVMSGMANNAGE_H

#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include <QObject>
#include <QString>
#include <QQueue>

#define DEV_MSG_DATA_LEN            (1024)  /*! 设备板件通讯报文长度 */
#define DEV_DATA_PT_NUM             (1024)  /*! 设备板件通讯数据点数目 */
#define DEV_DATA_BLK_LEN            (1024)  /*! 设备板件通讯数据块长度 */

#define GET_CF_FRAME_TYPE(cf)       (cf>>4)
#define SET_CF_FRAME_TYPE(cf, ft)   (cf | (ft<<4))

#define GET_CF_SQ_BIT(cf)           (cf & 0x01)
#define SET_CF_SQ_BIT(cf, sq)       (cf | (sq&0x01))

#define DEV_MSG_START_CHAR          (0x68)
#define DEV_MSG_END_CHAR            (0x16)

//#define DEVMSGMANNAGE_DEBUG_EN    (true)
#if defined(DEVMSGMANNAGE_DEBUG_EN) && DEVMSGMANNAGE_DEBUG_EN
#define devmsgmannage_debug     qDebug
#else
#define devmsgmannage_debug(...)
#endif

/*!
 * @brief 定义设备板件通讯报文控制域类型
 */
typedef enum
{
    CF_DataPt = 0x01,   /*!< 数据点上送（定时、召唤） */

    CF_YxChg,           /*!< 遥信变位 */

    CF_YcChg,           /*!< 遥测越限 */

    CF_DataBlk,         /*!< 数据块上送 */

    CF_DataPtAsk,       /*!< 数据总召唤 */

    CF_DataPtSet,       /*!< 数据下发 */

    CF_ACK,             /*!< 确认 */

    CF_NACK             /*!< 否认 */

} eCtrlField;

/*!
 * @brief 定义设备板件通讯报文头部结构
 */
#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint8_t     StartChar;  /*!< 启动字符 */

    uint16_t    MsgLen;     /*!< 报文长度 */

    uint8_t     CtrlField;  /*!< 控制域 */

} DevMsgHead_t;
#pragma pack(pop)

/*!
 * @brief 定义设备板件通讯报文尾部结构
 */
#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint8_t     CheckSum;   /*!< 帧校验和 */

    uint8_t     EndChar;    /*!< 结束字符 */

} DevMsgEnd_t;
#pragma pack(pop)

/*!
 * @brief 定义设备板件通讯报文结构
 */
#pragma pack(push)
#pragma pack(1)
typedef struct
{
    DevMsgHead_t    DevMsgHead;

    uint8_t         DevMsgData[DEV_MSG_DATA_LEN];

    DevMsgEnd_t     DevMsgEnd;

} DevMsg_t;
#pragma pack(pop)

/*!
 * @brief 定义设备板件通讯报文数据域结构（数据点 SQ=0）
 */
typedef struct
{
    uint16_t        StartAddr;                  /*!< 起始地址 */

    uint16_t        PtNum;                      /*!< 数据点数量 */

    float        PtData[DEV_DATA_PT_NUM];    /*!< 数据点数据 */

} DevDataPtSQ0_t;

/*!
 * @brief 定义设备板件通讯报文数据域结构（数据点 SQ=1）
 */
typedef struct
{
    uint16_t        PtNum;                      /*!< 数据点数量 */

    uint16_t        PtAddr[DEV_DATA_PT_NUM];    /*!< 数据点地址 */

    float           PtData[DEV_DATA_PT_NUM];    /*!< 数据点数据 */

} DevDataPtSQ1_t;

/*!
 * @brief 定义设备数据块类型
 */
typedef enum
{
    Blk_Torque = 0,

    Blk_Angle,

    Blk_Current

} eBlkType;

/*!
 * @brief 定义设备板件通讯报文数据域结构（数据块）
 */
typedef struct
{
    uint8_t         BlkType;                    /*!< 数据块类型 */

    uint16_t        BlkPktLen;                  /*!< 数据块包长度 */

    uint16_t        BlkLen;                     /*!< 数据块总长度 */

    uint8_t         BlkData[DEV_DATA_BLK_LEN];  /*!< 数据块 */

} DevDataBlk_t;


/*!
 * @brief 定义板间设备报文队列类型
 */
typedef enum
{
    DevMsgQ_Rcv = 0,

    DevMsgQ_Snd

} eDevMsgQ;

/*!
 * @brief 定义板间设备类型
 */
typedef enum
{
    DevType_Sgm0 = 0,

    DevType_Sgm1

} eDevType;

/*!
 * @brief 定义板间报文队列结构
 */
typedef struct
{
    eDevType    DevType;

    DevMsg_t    DevMsg;

} DevMsgQ_t;


class DevMsgMannage : public QObject
{
    Q_OBJECT
public:
    explicit DevMsgMannage(QObject *parent = 0);
    ~DevMsgMannage(void);

    void DevMsgClr(eDevMsgQ devMsgQType);
    void DevMsgPut(eDevMsgQ devMsgQType, DevMsgQ_t* devMsgQ);
    void DevMsgPop(eDevMsgQ devMsgQType, DevMsgQ_t* devMsgQ);
    bool DevMsgIsEmtpy(eDevMsgQ devMsgQType);

private:
    QQueue<DevMsgQ_t> m_DevRcvMsgQ;
    QQueue<DevMsgQ_t> m_DevSndMsgQ;
};

#endif // DEVMSGMANNAGE_H
