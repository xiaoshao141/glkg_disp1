#include "ctemmodbus.h"
#include <QByteArray>

CTemModbus::CTemModbus()
{
    m_ip = "192.168.1.100";
    dataP = 0x04;
    m_pTimer = new QTimer(this);
    m_pSkt = new QTcpSocket(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(rqstMsg()));
    connect(m_pSkt, SIGNAL(readyRead()), this, SLOT(rsvMsg()));

    m_pTimer->start(300);
}

CTemModbus::CTemModbus(QString ip)
{

}


void CTemModbus::rsvMsg()
{
    QByteArray rsv = m_pSkt->readAll();

    char* pRsv = rsv.data();

    qint16 val = (pRsv[9] << 8) + (quint8) pRsv[10];

    if (dataP == 4)
    {
        aTemp = val/10.0f;

    }
    else if (dataP == 9)
    {
        bTemp = val/10.0f;

    }
    else if (dataP == 14)
    {
        cTemp = val/10.0f;

    }

    dataP += 5;
    if (dataP > 14) dataP = 4;
}

char sndT[12] = {0, 0, 0, 0, 0, 0x6, 1, 3, 0, 0, 0, 1};

void CTemModbus::rqstMsg()
{
    if (m_pSkt->state() != QAbstractSocket::ConnectedState)
    {
        m_pSkt->connectToHost(m_ip, 502);
        bool ret = m_pSkt->waitForConnected(2000);
        if (!ret) return;
    }

    sndT[9] = dataP;
    QByteArray snd(sndT, 12);

    m_pSkt->write(snd);
    m_pSkt->waitForBytesWritten(100);

}

void CTemModbus::CtrlMask(int oper)
{


//    if (oper)
//    {
//        m_pSkt->write(snd);
//        m_pSkt->waitForBytesWritten(100);
//    }
//    else
//    {
//        m_pSkt->write(snd);
//        m_pSkt->waitForBytesWritten(100);
//    }
}

qreal aTemp = 10.0;
qreal bTemp = 10.0;
qreal cTemp = 10.0;
