#include "devmsgmannage.h"

DevMsgMannage::DevMsgMannage(QObject *parent) :
    QObject(parent)
{
}

DevMsgMannage::~DevMsgMannage(void)
{

}

void DevMsgMannage::DevMsgClr(eDevMsgQ devMsgQType)
{
    switch(devMsgQType)
    {
        case DevMsgQ_Rcv:
            m_DevRcvMsgQ.clear();
            break;
        case DevMsgQ_Snd:
            m_DevSndMsgQ.clear();
            break;
        default:
            break;
    }
}

void DevMsgMannage::DevMsgPut(eDevMsgQ devMsgQType, DevMsgQ_t* devMsgQ)
{
    switch(devMsgQType)
    {
        case DevMsgQ_Rcv:
            m_DevRcvMsgQ.enqueue(*devMsgQ);
            break;
        case DevMsgQ_Snd:
            m_DevSndMsgQ.enqueue(*devMsgQ);
            break;
        default:
            break;
    }
}

void DevMsgMannage::DevMsgPop(eDevMsgQ devMsgQType, DevMsgQ_t* devMsgQ)
{
    switch(devMsgQType)
    {
        case DevMsgQ_Rcv:
            *devMsgQ = m_DevRcvMsgQ.dequeue();
            break;
        case DevMsgQ_Snd:
            *devMsgQ = m_DevSndMsgQ.dequeue();
            break;
        default:
            break;
    }
}

bool DevMsgMannage::DevMsgIsEmtpy(eDevMsgQ devMsgQType)
{
    bool ret = false;

    switch(devMsgQType)
    {
        case DevMsgQ_Rcv:
            ret = m_DevRcvMsgQ.empty();
            break;
        case DevMsgQ_Snd:
            ret = m_DevSndMsgQ.empty();
            break;
        default:
            break;
    }

    return ret;
}
