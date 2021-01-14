#include "devcomm.h"

#include "stdint.h"
#include "string.h"
#include "stdio.h"

//#include "mainwnd.h"
#include "devmsgmannage.h"
#include "mainwindow.h"

QUdpSocket*     m_QUdpSocketRecv;
QUdpSocket*     m_QUdpSocketSend;

static unsigned int m_WaveDataCntA = 0;
static unsigned int m_WaveDataCntB = 0;
static unsigned int m_WaveDataCntC = 0;

static WaveData_t m_WaveData;

Devcomm::Devcomm()
{
}

Devcomm::~Devcomm()
{

}

int Devcomm::Init(void)
{
    m_WaveDataCntA = 0;
    m_WaveDataCntB = 0;
    m_WaveDataCntC = 0;
    memset(&m_WaveData, 0, sizeof(WaveData_t));

    m_DevRecvThread = new DevRecvThread();
    m_DevSendThread = new DevSendThread();
    m_DevProcThread = new DevProcThread();

    m_DevRecvThread->start();
    m_DevSendThread->start();
    m_DevProcThread->start();

    return 1;
}

/*!
 * @brief   第一部分
 * @detials UDP 板间通讯报文相关处理(Packet/Parse)
 */
bool Devcomm::DevMsgCheck(uint8_t* msg, uint16_t msgLen)
{
    bool ret = false;

    if((*(msg) == DEV_MSG_START_CHAR) &&
       (*(msg+msgLen-1) == DEV_MSG_END_CHAR))
    {
        ret = true;
    }

    return ret;
}

uint8_t Devcomm::DevMsgCalcSum(uint8_t *data, uint16_t dataLen)
{
    uint8_t calcSum = 0;

    for(unsigned int i=0; i<dataLen; i++)
    {
        calcSum += *(data + i);
    }

    return calcSum;
}

int Devcomm::DevMsgPacket(DevMsg_t* devMsg, uint8_t* msg, uint16_t *msgLen)
{
    int ret = -1;
    int offset = 0;
    unsigned int msgDataLen = 0;

    memcpy((uint8_t *)(msg+offset), (uint8_t *)(&devMsg->DevMsgHead), sizeof(DevMsgHead_t));
    offset += sizeof(DevMsgHead_t);

    msgDataLen = devMsg->DevMsgHead.MsgLen;
    memcpy((uint8_t *)(msg+offset), (uint8_t *)(&devMsg->DevMsgData), msgDataLen);
    offset += msgDataLen;

    memcpy((uint8_t *)(msg+offset), (uint8_t *)(&devMsg->DevMsgEnd), sizeof(DevMsgEnd_t));
    offset += sizeof(DevMsgEnd_t);

    *msgLen = (uint16_t)offset;
    ret = (int)(*msgLen);

    return ret;
}

int Devcomm::DevMsgParse(uint8_t* msg, uint16_t msgLen, DevMsg_t* devMsg)
{
    int ret = -1;
    int offset = 0;

    if(DevMsgCheck(msg, msgLen))
    {
        memcpy((uint8_t *)(&devMsg->DevMsgHead), (uint8_t *)(msg+offset), sizeof(DevMsgHead_t));
        //_SWAP_HILW_2B_((uint8_t *)(&devMsg->DevMsgHead.MsgLen));
        offset += sizeof(DevMsgHead_t);

        memcpy((uint8_t *)(&devMsg->DevMsgData), (uint8_t *)(msg+offset), devMsg->DevMsgHead.MsgLen);
        offset += devMsg->DevMsgHead.MsgLen;

        memcpy((uint8_t *)(&devMsg->DevMsgEnd), (uint8_t *)(msg+offset), sizeof(DevMsgEnd_t));
        offset += sizeof(DevMsgEnd_t);

        ret = 1;
    }

    return ret;
}

int Devcomm::DevDataPtSQ0Packet(DevDataPtSQ0_t* devDataPt, uint8_t* data, uint16_t *dataLen)
{
    int ret = -1;
    int offset = 0;

    memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataPt->StartAddr), sizeof(uint16_t));
    //_SWAP_HILW_2B_((uint8_t *)(data+offset));
    offset += sizeof(uint16_t);

    memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataPt->PtNum), sizeof(uint8_t));
    //_SWAP_HILW_2B_((uint8_t *)(data+offset));
    offset += sizeof(uint16_t);

    for(unsigned int i=0; i<devDataPt->PtNum; i++)
    {
        *(float *)(data+offset) = devDataPt->PtData[i];
        //_SWAP_HILW_4B_((uint8_t *)(data+offset));
        offset += sizeof(float);
    }

    *dataLen = offset;
    ret = (int)dataLen;

    return ret;
}

int Devcomm::DevDataPtSQ0Parse(uint8_t* data, uint16_t dataLen, DevDataPtSQ0_t* devDataPt)
{
    int         ret = -1;
    int         offset = 0;
    uint16_t    dataMsgLen = 0;

    memcpy((uint8_t *)(&devDataPt->StartAddr), (uint8_t *)(data+offset), sizeof(uint16_t));
    //_SWAP_HILW_2B_((uint8_t *)(&devDataPt->StartAddr));
    offset += sizeof(uint16_t);

    memcpy((uint8_t *)(&devDataPt->PtNum), (uint8_t *)(data+offset), sizeof(uint8_t));
    //_SWAP_HILW_2B_((uint8_t *)(&devDataPt->PtNum));
    offset += sizeof(uint16_t);

    dataMsgLen = devDataPt->PtNum*sizeof(float) + offset;
    if(dataMsgLen <= dataLen)
    {
        for(unsigned int i=0; i<devDataPt->PtNum; i++)
        {
            devDataPt->PtData[i] = *(float *)(data+offset);
            //_SWAP_HILW_4B_((uint8_t *)(&devDataPt->PtData[i]));
            offset += sizeof(float);
        }

        ret = 1;
    }

    return ret;
}

int Devcomm::DevDataPtSQ1Packet(DevDataPtSQ1_t* devDataPt, uint8_t* data, uint16_t* dataLen)
{
    int ret = -1;
    int offset = 0;

    memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataPt->PtNum), sizeof(uint16_t));
    //_SWAP_HILW_2B_((uint8_t *)(data+offset));
    offset += sizeof(uint16_t);

    for(unsigned int i=0; i<devDataPt->PtNum; i++)
    {
        memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataPt->PtAddr), sizeof(uint16_t));
        //_SWAP_HILW_2B_((uint8_t *)(data+offset));
        offset += sizeof(uint16_t);

        memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataPt->PtData), sizeof(uint16_t));
        //_SWAP_HILW_4B_((uint8_t *)(data+offset));
        offset += sizeof(float);
    }

    *dataLen = offset;
    ret = (int)(*dataLen);

    return ret;
}

int Devcomm::DevDataPtSQ1Parse(uint8_t* data, uint16_t dataLen, DevDataPtSQ1_t* devDataPt)
{
    int         ret = -1;
    int         offset = 0;
    uint16_t    dataMsgLen = 0;

    memcpy((uint8_t *)(&devDataPt->PtNum), (uint8_t *)(data+offset), sizeof(uint16_t));
    //_SWAP_HILW_2B_((uint8_t *)(&devDataPt->PtNum));
    offset += sizeof(uint16_t);

    dataMsgLen = devDataPt->PtNum*sizeof(uint16_t) + devDataPt->PtNum*sizeof(float) + offset;
    if(dataMsgLen <= dataLen)
    {
        for(unsigned int i=0; i<devDataPt->PtNum; i++)
        {
            memcpy((uint8_t *)(&devDataPt->PtAddr[i]), (uint8_t *)(data+offset), sizeof(uint16_t));
            //_SWAP_HILW_2B_((uint8_t *)(&devDataPt->PtAddr));
            offset += sizeof(uint16_t);

            memcpy((uint8_t *)(&devDataPt->PtData[i]), (uint8_t *)(data+offset), sizeof(float));
            //_SWAP_HILW_2B_((uint8_t *)(&devDataPt->PtData));
            offset += sizeof(float);
        }

        ret = 1;
    }
    else
    {
        devcomm_debug("SQ1 Data Field Parse Errored \r\n");
    }

    return ret;
}

int Devcomm::DevDataBlkPacket(DevDataBlk_t* devDataBlk, uint8_t* data, uint16_t* dataLen)
{
    int ret = -1;
    int offset = 0;

    memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataBlk->BlkType), sizeof(uint8_t));
    offset++;

    memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataBlk->BlkPktLen), sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataBlk->BlkLen), sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy((uint8_t *)(data+offset), (uint8_t *)(&devDataBlk->BlkData), devDataBlk->BlkPktLen);
    offset += devDataBlk->BlkPktLen;

    *dataLen = offset;
    ret = (int)*dataLen;

    return ret;
}

int Devcomm::DevDataBlkParse(uint8_t* data, uint16_t dataLen, DevDataBlk_t* devDataBlk)
{
    int         ret = -1;
    int         offset = 0;
    uint16_t    dataMsgLen = 0;

    memcpy((uint8_t *)(&devDataBlk->BlkType), (uint8_t *)(data+offset), sizeof(uint8_t));
    offset++;

    memcpy((uint8_t *)(&devDataBlk->BlkPktLen), (uint8_t *)(data+offset), sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy((uint8_t *)(&devDataBlk->BlkLen), (uint8_t *)(data+offset), sizeof(uint16_t));
    offset += sizeof(uint16_t);

    dataMsgLen = offset + devDataBlk->BlkPktLen;
    if(dataMsgLen <= dataLen)
    {
        memcpy((uint8_t *)(&devDataBlk->BlkData), (uint8_t *)(data+offset), devDataBlk->BlkPktLen);
        offset += devDataBlk->BlkPktLen;

        ret = 1;
    }

    return ret;
}

int Devcomm::DevGenDataPtSQ0(eCtrlField ctrlCmd, DevDataPtSQ0_t *devDataPt, DevMsg_t *devMsg)
{
    int         ret = -1;
    uint16_t    dataLen = 0;
    int         dataOffset = 0;

    dataLen = devDataPt->PtNum * sizeof(float);

    devMsg->DevMsgHead.StartChar = DEV_MSG_START_CHAR;
    devMsg->DevMsgHead.MsgLen = sizeof(uint16_t) + sizeof(uint16_t) + dataLen;
    devMsg->DevMsgHead.CtrlField = (ctrlCmd<<4) | 0x00;

    memcpy((uint8_t *)devMsg->DevMsgData+dataOffset, (uint8_t *)(&devDataPt->StartAddr), sizeof(uint16_t));
    dataOffset += sizeof(uint16_t);

    memcpy((uint8_t *)devMsg->DevMsgData+dataOffset, (uint8_t *)(&devDataPt->PtNum), sizeof(uint16_t));
    dataOffset += sizeof(uint16_t);

    memcpy((uint8_t *)devMsg->DevMsgData+dataOffset, (uint8_t *)devDataPt->PtData, dataLen);

    devMsg->DevMsgEnd.CheckSum = DevMsgCalcSum((uint8_t *)devMsg->DevMsgData, dataLen);
    devMsg->DevMsgEnd.EndChar = DEV_MSG_END_CHAR;
    ret = 1;

    return ret;
}

int Devcomm::DevGenDataPtSQ1(eCtrlField ctrlCmd, DevDataPtSQ1_t *devDataPt, DevMsg_t *devMsg)
{
    int         ret = -1;
    uint16_t    dataLen = 0;
    int         dataOffset = 0;

    dataLen = devDataPt->PtNum * sizeof(uint16_t) + devDataPt->PtNum * sizeof(float);

    devMsg->DevMsgHead.StartChar = DEV_MSG_START_CHAR;
    devMsg->DevMsgHead.MsgLen = sizeof(uint16_t) + dataLen;
    devMsg->DevMsgHead.CtrlField = (ctrlCmd<<4) | 0x01;

    memcpy((uint8_t *)devMsg->DevMsgData+dataOffset, (uint8_t*)(&devDataPt->PtNum), sizeof(uint16_t));
    dataOffset += sizeof(uint16_t);

    for(unsigned int i=0; i<devDataPt->PtNum; i++)
    {
        memcpy((uint8_t *)devMsg->DevMsgData+dataOffset, (uint8_t *)devDataPt->PtAddr, sizeof(uint16_t));
        dataOffset += sizeof(uint16_t);

        memcpy((uint8_t *)devMsg->DevMsgData+dataOffset, (uint8_t *)devDataPt->PtData, sizeof(float));
        dataOffset += sizeof(float);
    }

    devMsg->DevMsgEnd.CheckSum = DevMsgCalcSum((uint8_t *)devMsg->DevMsgData, dataLen);
    devMsg->DevMsgEnd.EndChar = DEV_MSG_END_CHAR;
    ret = 1;

    return ret;
}

int Devcomm::DevGenAskAll(DevMsg_t *devMsg)
{
    int         ret = -1;

    devMsg->DevMsgHead.StartChar = DEV_MSG_START_CHAR;
    devMsg->DevMsgHead.MsgLen = 0x00;
    devMsg->DevMsgHead.CtrlField = CF_DataPtAsk;

    devMsg->DevMsgEnd.CheckSum = 0x00;
    devMsg->DevMsgEnd.EndChar = DEV_MSG_END_CHAR;
    ret = 1;

    return ret;
}

int Devcomm::DevGenRplyAck(DevMsg_t* devMsg)
{
    int         ret = -1;

    devMsg->DevMsgHead.StartChar = DEV_MSG_START_CHAR;
    devMsg->DevMsgHead.MsgLen = 0x00;
    devMsg->DevMsgHead.CtrlField = CF_ACK;

    devMsg->DevMsgEnd.CheckSum = 0x00;
    devMsg->DevMsgEnd.EndChar = DEV_MSG_END_CHAR;
    ret = 1;

    return ret;
}

int Devcomm::DevGenRplyNAck(DevMsg_t* devMsg)
{
    int ret = -1;

    devMsg->DevMsgHead.StartChar = DEV_MSG_START_CHAR;
    devMsg->DevMsgHead.MsgLen = 0x00;
    devMsg->DevMsgHead.CtrlField = CF_NACK;

    devMsg->DevMsgEnd.CheckSum = 0x00;
    devMsg->DevMsgEnd.EndChar = DEV_MSG_END_CHAR;
    ret = 1;

    return ret;
}

/*!
 * @brief   第二部分
 * @detials UDP板间通讯接受报文相关处理
 */
void Devcomm::DevMsgProcDataPt(DevMsg_t* devMsg)
{
    DevDataPtSQ0_t devDataPtSQ0;
    DevDataPtSQ1_t devDataPtSQ1;

    devcomm_debug("CtrlField: %d", devMsg->DevMsgHead.CtrlField);

    if(GET_CF_SQ_BIT(devMsg->DevMsgHead.CtrlField) == 0x00)
    {
        g_Devcomm->DevDataPtSQ0Parse(devMsg->DevMsgData,
                                     devMsg->DevMsgHead.MsgLen,
                                     &devDataPtSQ0);


        devcomm_debug("SQ0 MSG PT NUM: %d", devDataPtSQ0.PtNum);

        if(devDataPtSQ0.PtNum <= DEV_DATA_PT_NUM)
        {
            for(unsigned int i=0; i<devDataPtSQ0.PtNum; i++)
            {
                devcomm_debug("%04d    %04f",
                              devDataPtSQ0.StartAddr+i,
                              devDataPtSQ0.PtData[i]);

                g_Devoper->setDevDBObjData(devDataPtSQ0.StartAddr+i,
                                           &devDataPtSQ0.PtData[i]);

                if(devDataPtSQ0.StartAddr+i == YC_RTTOR_ADDR)
                    g_Devwave->UpdateRtWave(2,  (uint16_t)devDataPtSQ0.PtData[i]);
                else if(devDataPtSQ0.StartAddr+i == YC_RTCUR_ADDR)
                    g_Devwave->UpdateRtWave(1,  (uint16_t)devDataPtSQ0.PtData[i]);
                else if(devDataPtSQ0.StartAddr+i == YC_RTANG_ADDR)
                    g_Devwave->UpdateRtWave(0,  (uint16_t)devDataPtSQ0.PtData[i]);
            }

            devcomm_debug("Update Realtime Data \r\n");
        }
    }
    else
    {
        g_Devcomm->DevDataPtSQ1Parse(devMsg->DevMsgData,
                                     devMsg->DevMsgHead.MsgLen,
                                     &devDataPtSQ1);

        devcomm_debug("SQ1 MSG PT NUM: %d", devDataPtSQ1.PtNum);

        if(devDataPtSQ1.PtNum <= DEV_DATA_PT_NUM)
        {
            for(unsigned int i=0; i<devDataPtSQ1.PtNum; i++)
            {
                devcomm_debug("%04d    %04f",
                              devDataPtSQ1.PtAddr[i],
                              devDataPtSQ1.PtData[i]);

                g_Devoper->setDevDBObjData(devDataPtSQ1.PtAddr[i],
                                           &devDataPtSQ1.PtData[i]);

                if(devDataPtSQ1.PtAddr[i] == YC_RTTOR_ADDR)
                    g_Devwave->UpdateRtWave(2,  (uint16_t)devDataPtSQ1.PtData[i]);
                else if(devDataPtSQ1.PtAddr[i] == YC_RTCUR_ADDR)
                    g_Devwave->UpdateRtWave(1,  (uint16_t)devDataPtSQ1.PtData[i]);
                else if(devDataPtSQ1.PtAddr[i] == YC_RTANG_ADDR)
                    g_Devwave->UpdateRtWave(0,  (uint16_t)devDataPtSQ1.PtData[i]);
            }

            devcomm_debug("Update Realtime Data  \r\n");
        }
    }
}

void Devcomm::DevMsgProcYxChg(DevMsg_t* devMsg)
{
    DevDataPtSQ0_t  devDataPtSQ0;
    DevDataPtSQ1_t  devDataPtSQ1;
    RptRecord_t     rptRecord;
    QString         rptDesc;
    QString         objDesc;

    if(GET_CF_SQ_BIT(devMsg->DevMsgHead.CtrlField) == 0x00)
    {
        g_Devcomm->DevDataPtSQ0Parse(devMsg->DevMsgData,
                                     devMsg->DevMsgHead.MsgLen,
                                     &devDataPtSQ0);

        devcomm_debug("SQ0 MSG PT NUM: %d", devDataPtSQ0.PtNum);

        if(devDataPtSQ0.PtNum <= DEV_DATA_PT_NUM)
        {
            for(unsigned int i=0; i<devDataPtSQ0.PtNum; i++)
            {
                devcomm_debug("%04d    %04f",
                              devDataPtSQ0.StartAddr+i,
                              devDataPtSQ0.PtData[i]);

                g_Devoper->setDevDBObjData(devDataPtSQ0.StartAddr+i,
                                           &devDataPtSQ0.PtData[i]);

                g_Devoper->getDevObjDesc(devDataPtSQ0.StartAddr+i, &objDesc);
                rptDesc.sprintf("遥信变位告警 [%s]", objDesc.toLatin1().data());

                rptRecord.RptType = RptType_Event;
                rptRecord.RptDesc = rptDesc;
                rptRecord.RptDateTime = QDateTime::currentDateTime();
                rptRecord.RptVal = devDataPtSQ0.PtData[i];
                g_Devreport->RptPutIn(RptType_Event, &rptRecord);

                DevYxChgUpdate(devDataPtSQ0.StartAddr+i, devDataPtSQ0.PtData[i]);

                devcomm_debug("%s \r\n", rptDesc.toLatin1().data());
            }
        }
    }
    else
    {
        g_Devcomm->DevDataPtSQ1Parse(devMsg->DevMsgData,
                                     devMsg->DevMsgHead.MsgLen,
                                     &devDataPtSQ1);

        devcomm_debug("SQ1 MSG PT NUM: %d", devDataPtSQ1.PtNum);

        if(devDataPtSQ1.PtNum <= DEV_DATA_PT_NUM)
        {
            for(unsigned int i=0; i<devDataPtSQ1.PtNum; i++)
            {
                devcomm_debug("%04d    %04f",
                              devDataPtSQ1.PtAddr[i],
                              devDataPtSQ1.PtData[i]);

                g_Devoper->setDevDBObjData(devDataPtSQ1.PtAddr[i],
                                           &devDataPtSQ1.PtData[i]);

                g_Devoper->getDevObjDesc(devDataPtSQ1.PtAddr[i], &objDesc);
                rptDesc.sprintf("遥信变位告警 [%s]", objDesc.toLatin1().data());

                rptRecord.RptType = RptType_Event;
                rptRecord.RptDesc = rptDesc;
                rptRecord.RptDateTime = QDateTime::currentDateTime();
                rptRecord.RptVal = devDataPtSQ1.PtData[i];
                g_Devreport->RptPutIn(RptType_Event, &rptRecord);

                DevYxChgUpdate(devDataPtSQ1.PtAddr[i], devDataPtSQ1.PtData[i]);

                devcomm_debug("%s \r\n", rptDesc.toLatin1().data());
            }
        }
    }
}

void Devcomm::DevMsgProcYcChg(DevMsg_t* devMsg)
{
    DevDataPtSQ0_t  devDataPtSQ0;
    DevDataPtSQ1_t  devDataPtSQ1;
    RptRecord_t     rptRecord;
    QString         rptDesc;
    QString         objDesc;

    if(GET_CF_SQ_BIT(devMsg->DevMsgHead.CtrlField) == 0x00)
    {
        g_Devcomm->DevDataPtSQ0Parse(devMsg->DevMsgData,
                                     devMsg->DevMsgHead.MsgLen,
                                     &devDataPtSQ0);

        if(devDataPtSQ0.PtNum <= DEV_DATA_PT_NUM)
        {
            for(unsigned int i=0; i<devDataPtSQ0.PtNum; i++)
            {
                g_Devoper->setDevDBObjData(devDataPtSQ0.StartAddr+i,
                                           &devDataPtSQ0.PtData[i]);

                g_Devoper->getDevObjDesc(devDataPtSQ0.StartAddr+i, &objDesc);
                rptDesc.sprintf("遥测越限告警 [%s]", objDesc.toLatin1().data());

                rptRecord.RptType = RptType_Event;
                rptRecord.RptDesc = rptDesc;
                rptRecord.RptDateTime = QDateTime::currentDateTime();
                rptRecord.RptVal = devDataPtSQ0.PtData[i];
                g_Devreport->RptPutIn(RptType_Event, &rptRecord);

                DevYcChgUpdate(devDataPtSQ0.StartAddr+i, devDataPtSQ0.PtData[i]);

                devcomm_debug("%s \r\n", rptDesc.toLatin1().data());
            }
        }
    }
    else
    {
        g_Devcomm->DevDataPtSQ1Parse(devMsg->DevMsgData,
                                     devMsg->DevMsgHead.MsgLen,
                                     &devDataPtSQ1);

        if(devDataPtSQ1.PtNum <= DEV_DATA_PT_NUM)
        {
            for(unsigned int i=0; i<devDataPtSQ1.PtNum; i++)
            {
                g_Devoper->setDevDBObjData(devDataPtSQ1.PtAddr[i],
                                           &devDataPtSQ1.PtData[i]);

                g_Devoper->getDevObjDesc(devDataPtSQ1.PtAddr[i], &objDesc);
                rptDesc.sprintf("遥测越限告警 [%s]", objDesc.toLatin1().data());

                rptRecord.RptType = RptType_Event;
                rptRecord.RptDesc = rptDesc;
                rptRecord.RptDateTime = QDateTime::currentDateTime();
                rptRecord.RptVal = devDataPtSQ1.PtData[i];
                g_Devreport->RptPutIn(RptType_Event, &rptRecord);

                DevYcChgUpdate(devDataPtSQ1.PtAddr[i], devDataPtSQ1.PtData[i]);

                devcomm_debug("%s \r\n", rptDesc.toLatin1().data());
            }
        }
    }
}

void Devcomm::DevMsgProcDataBlk(DevMsg_t* devMsg)
{
    QString         waveFileName;
    unsigned int    waveFileSeq = 0;
    QDateTime       nowDateTime;
    DevDataBlk_t    devDataBlk;
    RptRecord_t     rptRecord;

    memset(&devDataBlk, 0, sizeof(DevDataBlk_t));
    DevDataBlkParse(devMsg->DevMsgData, devMsg->DevMsgHead.MsgLen, &devDataBlk);

    switch(devDataBlk.BlkType)
    {
        case Blk_Torque:
            if((DEV_WAVE_BUFSIZE-m_WaveDataCntA >= devDataBlk.BlkPktLen) && \
               (m_WaveDataCntA <= DEV_WAVE_BUFSIZE))
            {
                memcpy((uint8_t *)(m_WaveData.Tor)+m_WaveDataCntA, devDataBlk.BlkData, devDataBlk.BlkPktLen);
                m_WaveDataCntA += devDataBlk.BlkPktLen;

                devcomm_debug("Wave Data: Curr %d \r\n", m_WaveDataCntA);
            }
            break;
        case Blk_Angle:
            if((DEV_WAVE_BUFSIZE-m_WaveDataCntB >= devDataBlk.BlkPktLen) && \
               (m_WaveDataCntB <= DEV_WAVE_BUFSIZE))
            {
                memcpy((uint8_t *)(m_WaveData.Ang)+m_WaveDataCntB, devDataBlk.BlkData, devDataBlk.BlkPktLen);
                m_WaveDataCntB += devDataBlk.BlkPktLen;

                devcomm_debug("Wave Data: Ang %d \r\n", m_WaveDataCntB);
            }
            break;
        case Blk_Current:
            if((DEV_WAVE_BUFSIZE-m_WaveDataCntC >= devDataBlk.BlkPktLen) && \
               (m_WaveDataCntC <= DEV_WAVE_BUFSIZE))
            {
                memcpy((uint8_t *)(m_WaveData.Cur)+m_WaveDataCntC, devDataBlk.BlkData, devDataBlk.BlkPktLen);
                m_WaveDataCntC += devDataBlk.BlkPktLen;

                devcomm_debug("Wave Data: Cur %d \r\n", m_WaveDataCntC);
            }
            break;
        default:
            break;
    }

    if((m_WaveDataCntA == DEV_WAVE_BUFSIZE) && \
       (m_WaveDataCntB == DEV_WAVE_BUFSIZE) && \
       (m_WaveDataCntC == DEV_WAVE_BUFSIZE))
    {
        if(g_Devwave->swState())
            m_WaveData.BrkOper = BrkOper_On;
        else
            m_WaveData.BrkOper = BrkOper_Off;

        waveFileSeq = g_Devwave->WaveFileGetNum();
        nowDateTime = QDateTime::currentDateTime();

        waveFileName.sprintf(DEV_WAVE_FILE_FORMAT,
                             waveFileSeq,
                             nowDateTime.date().year(),
                             nowDateTime.date().month(),
                             nowDateTime.date().day(),
                             nowDateTime.time().hour(),
                             nowDateTime.time().minute(),
                             nowDateTime.time().second(),
                             nowDateTime.time().msec());
        g_Devwave->WaveFileImport(waveFileName, &m_WaveData);

        m_WaveDataCntA = 0;
        m_WaveDataCntB = 0;
        m_WaveDataCntC = 0;

        switch(m_WaveData.BrkOper)
        {
            case BrkOper_On:
                rptRecord.RptDesc = "隔离开关合闸操作录波";
                rptRecord.RptVal = 0x01;
                break;
            case BrkOper_Off:
                rptRecord.RptDesc = "隔离开关分闸操作录波";
                rptRecord.RptVal = 0x01;
                break;
            default:
                break;
        }
        rptRecord.RptType = RptType_Wave;
        rptRecord.RptDateTime = nowDateTime;
        g_Devreport->RptPutIn(RptType_Event, &rptRecord);

        devcomm_debug("保存录波文件成功 \r\n");
    }
}

void Devcomm::DevMsgProcAck(DevMsg_t* devMsg){}

void Devcomm::DevMsgProcNAck(DevMsg_t* devMsg){}

void Devcomm::DevYxChgUpdate(uint16_t yxAddr, uint32_t yxData)
{
    // +++ 相关遥信点变位处理

    /* 断路器分合闸操作记录 */
}

void Devcomm::DevYcChgUpdate(uint16_t ycAddr, uint32_t ycData)
{
    // +++ 相关遥测点越限处理
}

/*
 * @brief   第三部分
 * @details UDP 板间通讯相关线程（RECV、SEND、PROC）
 */
DevRecvThread::DevRecvThread(void){}

DevSendThread::DevSendThread(void){}

DevProcThread::DevProcThread(void){}

void DevRecvThread::run(void)
{
    QByteArray      msgByteArray;
    DevMsgQ_t       devMsgQ;
    QString         recvPeerAddr;
    uint8_t         msgBuf[DEV_MSG_DATA_LEN];
    uint16_t        msgLen = 0;

    bool            ret = false;
    QHostAddress    hostAddress;
    quint16         hostPort;

    m_QUdpSocketRecv = new QUdpSocket(this);

    hostAddress = QHostAddress(DEV_HOST_IPADDR);
    hostPort = DEV_HOST_PORT;

    ret = m_QUdpSocketRecv->bind(hostAddress, hostPort);
    if(ret)
        devcomm_debug("Udp Host Bind Successed %s:%d\r\n",
               hostAddress.toString().toLatin1().data(),
               hostPort);
    else
        devcomm_debug("Udp Host Bind Errored %s:%d\r\n",
               hostAddress.toString().toLatin1().data(),
               hostPort);

    while(1)
    {
        if(m_QUdpSocketRecv->hasPendingDatagrams())
            //hasPendingDatagrams如果至少有一个数据报等待读取，则返回true;否则返回false。
        {
            devcomm_debug("Thread: [RECV DATA] \r\n");

            msgByteArray.resize(m_QUdpSocketRecv->pendingDatagramSize());
            //pendingDatagramSize返回第一个挂起的UDP数据报的大小。如果没有可用的数据报，则此函数返回-1。
            m_QUdpSocketRecv->readDatagram(msgByteArray.data(), msgByteArray.size());
            //接收不大于maxSize字节的数据报，并将其存储在数据中。
            //发送方的主机地址和端口存储在*address和*port中(除非指针为0)。

            if(msgByteArray.size() > 0)
            {
                recvPeerAddr = m_QUdpSocketRecv->peerAddress().toString();
                //peerAddress如果套接字处于ConnectedState状态，则返回已连接的对等端地址;否则返回QHostAddress::零。
                devcomm_debug("Recv Peer Addr: %s \r\n", recvPeerAddr.toLatin1().data());

                memset(msgBuf, 0, sizeof(msgBuf));
                msgLen = msgByteArray.size();
                for(int i=0; i<msgLen; i++)
                {
                    msgBuf[i] = (uint8_t)msgByteArray.at(i);
                }

                devcomm_debug("<- msglen: %d \r\n   msgdata: %s \r\n", msgLen, msgByteArray.toHex().data());

                if(g_Devcomm->DevMsgParse(msgBuf, msgLen, &(devMsgQ.DevMsg)) >= 0)
                {
                    g_DevMsgMannage->DevMsgPut(DevMsgQ_Rcv, &devMsgQ);
                }
            }
        }
        else
        {
            msleep(10);
        }
    }
}

void DevSendThread::run(void)
{
    DevMsgQ_t   devMsgQ;
    uint8_t     msgBuf[DEV_MSG_DATA_LEN];
    uint16_t    msgLen = 0;

    m_QUdpSocketSend = new QUdpSocket(this);

    while(1)
    {
        if(!g_DevMsgMannage->DevMsgIsEmtpy(DevMsgQ_Snd))
        {
            devcomm_debug("Thread: [SEND DATA] \r\n");
            memset(&devMsgQ, 0, sizeof(DevMsgQ_t));
            g_DevMsgMannage->DevMsgPop(DevMsgQ_Snd, &devMsgQ);

            g_Devcomm->DevMsgPacket(&(devMsgQ.DevMsg), msgBuf, &msgLen);
            QByteArray* msgByteArray = new QByteArray((const char *)msgBuf, msgLen);
            devcomm_debug("-> %s \r\n", msgByteArray->toHex().data());

            switch(devMsgQ.DevType)
            {
                case DevType_Sgm0:
                    m_QUdpSocketSend->writeDatagram(msgByteArray->data(),
                                                    QHostAddress(DEV_SGM0_IPADDR),
                                                    DEV_SGM0_PORT);
                    break;
                case DevType_Sgm1:
                    m_QUdpSocketSend->writeDatagram(msgByteArray->data(),
                                                    QHostAddress(DEV_SGM1_IPADDR),
                                                    DEV_SGM1_PORT);
                    break;
                default:
                    break;
            }
        }
        else
        {
            msleep(10);
        }
    }
}

void DevProcThread::run(void)
{
    DevMsgQ_t   devMsgQ;

    while(1)
    {
        if(!g_DevMsgMannage->DevMsgIsEmtpy(DevMsgQ_Rcv))
        {
            devcomm_debug("Thread: [PROC DATA] \r\n");

            g_DevMsgMannage->DevMsgPop(DevMsgQ_Rcv, &devMsgQ);

            switch(GET_CF_FRAME_TYPE(devMsgQ.DevMsg.DevMsgHead.CtrlField))
            {
                case CF_DataPt:     // 数据点上送（定时、召唤）
                    g_Devcomm->DevMsgProcDataPt(&(devMsgQ.DevMsg));
                    break;
                case CF_YxChg:      // 遥信变位
                    g_Devcomm->DevMsgProcYxChg(&(devMsgQ.DevMsg));
                    break;
                case CF_YcChg:      // 遥测越限
                    g_Devcomm->DevMsgProcYcChg(&(devMsgQ.DevMsg));
                    break;
                case CF_DataBlk:    // 数据块上送
                    g_Devcomm->DevMsgProcDataBlk(&(devMsgQ.DevMsg));
                    break;
                case CF_ACK:        // 确认
                    g_Devcomm->DevMsgProcAck(&(devMsgQ.DevMsg));
                    break;
                case CF_NACK:       // 否认
                    g_Devcomm->DevMsgProcNAck(&(devMsgQ.DevMsg));
                    break;
                default:
                    break;
            }
        }
        else
        {
            msleep(10);
        }
    }
}
