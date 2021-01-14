#include "devpdcomm.h"

//#include "mainwnd.h"
#include "mainwindow.h"

QTcpSocket*     m_DevpdTcpClient;
bool            m_ConnectOk;

Devpdcomm::Devpdcomm(QObject *parent) :
    QObject(parent)
{
}

Devpdcomm::~Devpdcomm()
{

}

void Devpdcomm::Init(void)
{
    m_DevpdUpdtThread = new DevpdUpdtThread();

    m_DevpdUpdtThread->start();
}

/*!
 * @brief  第二部分
 * @detail 局放TCP通讯相关线程处理部分
 */

DevpdUpdtThread::DevpdUpdtThread(void)
{
}

void DevpdUpdtThread::run(void)
{
    RST rst_msg;

    rst_msg.nType = 0x00;
    rst_msg.nChanIdx = 0x00;

    m_DevpdTcpClient = new QTcpSocket(this);
    m_DevpdTcpClient->abort();

    connect(m_DevpdTcpClient, SIGNAL(readyRead()), this, SLOT(onReadMsg()));
    connect(m_DevpdTcpClient, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(m_DevpdTcpClient, SIGNAL(disconnected()), this, SLOT(onDisConnect()));

    m_ConnectOk = false;

    while(1)
    {
        if(!m_ConnectOk && m_DevpdTcpClient!=NULL)
        {
            m_DevpdTcpClient->connectToHost(DEVPD_TCP_IPADDR, DEVPD_TCP_PORT);

            m_ConnectOk = m_DevpdTcpClient->waitForConnected(5000);
            if(m_ConnectOk)
                devpdcomm_debug("DEV PD TCP Connect Successed \r\n");
            else
                devpdcomm_debug("DEV PD TCP Connect Errored \r\n");
        }
        else if(m_ConnectOk)
        {
            if(m_DevpdTcpClient->isWritable())
            {
                int ret = (int)m_DevpdTcpClient->write((const char *)&rst_msg, sizeof(RST));
                if(ret > 0)
                    devpdcomm_debug("Thread [Update] Send Req Successed \r\n");
            }

            m_DevpdTcpClient->waitForReadyRead(DEVPD_UPDATE_PERIOD);
        }
        else
        {
            sleep(DEVPD_UPDATE_PERIOD);
        }
    }
}

void DevpdUpdtThread::onReadMsg(void)
{
    QByteArray  datagram;
    PEAK_RSP    peak_rsp;

    while (m_DevpdTcpClient->bytesAvailable() > 0)
    {
        datagram.clear();
        datagram.resize(m_DevpdTcpClient->bytesAvailable());
        m_DevpdTcpClient->read(datagram.data(), datagram.size());
    }

    /*!
     * @brief 暂时只支持读取局放DB值，PRPD/PRPS暂时不支持
     */
    if(datagram.size() == sizeof(PEAK_RSP))
    {
        devpdcomm_debug("DEV PD Read Msg Successed   \r\n   \
                       UHF Value: %f db             \r\n    \
                       AE  Value: %f db             \r\n    \
                       TEV Value: %f db             \r\n",  \
                       peak_rsp.fPeak[0],                   \
                       peak_rsp.fPeak[1],                   \
                       peak_rsp.fPeak[2]);

        memcpy((uint8_t *)&peak_rsp, datagram.data(), datagram.size());

        g_Devoper->setDevDBObjData(DEVPD_UHF_OBJADDR, &peak_rsp.fPeak[0]);
        g_Devoper->setDevDBObjData(DEVPD_AE_OBJADDR, &peak_rsp.fPeak[1]);
        g_Devoper->setDevDBObjData(DEVPD_TEV_OBJADDR, &peak_rsp.fPeak[2]);
    }
    else
    {
        devpdcomm_debug("DEV PD Read Msg Errored \r\n");
    }
}

void DevpdUpdtThread::onConnect(void)
{
}

void DevpdUpdtThread::onDisConnect(void)
{
    m_ConnectOk = false;
}
