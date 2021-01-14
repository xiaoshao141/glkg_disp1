#include "devreport.h"
#include "sgmdb.h"
//#include "mainwnd.h"

#include <QDebug>

Devreport::Devreport()
{
}

Devreport::~Devreport()
{
}

int Devreport::RptInit(void)
{

    return 1;
}

int Devreport::RptClear(eRptType rptType)
{
    int ret = -1;
    QString sqlStr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    switch(rptType)
    {
        case RptType_Oper:
            sqlStr.sprintf("delete * from RPT_OPT_TAB");
            break;
        case RptType_Event:
            sqlStr.sprintf("delete * from RPT_EVT_TAB");
            break;
        case RptType_Action:
            sqlStr.sprintf("delete * from RPT_ACT_TAB");
            break;
        case RptType_Wave:
            sqlStr.sprintf("delete * from RPT_WAVE_TAB");
            break;
        default:
            break;
    }

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
        return ret;
    }

    return ret;
}

int Devreport::RptPutIn(eRptType rptType, RptRecord_t* rptRecord)
{
    int ret = -1;
    QString sqlStr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    switch(rptType)
    {
        case RptType_Oper:
            sqlStr.sprintf("insert into RPT_OPT_TAB (ID,TYPE,DESC,TIME,VAL) values (%d,%d,%s,datetime('%04d-%02d-%02d %02d:%02d:%02d'),%d)",
                           m_RptNum[rptType],
                           rptRecord->RptType,
                           rptRecord->RptDesc.toUtf8().data(),
                           rptRecord->RptDateTime.date().year(),
                           rptRecord->RptDateTime.date().month(),
                           rptRecord->RptDateTime.date().day(),
                           rptRecord->RptDateTime.time().hour(),
                           rptRecord->RptDateTime.time().minute(),
                           rptRecord->RptDateTime.time().second(),
                           rptRecord->RptVal);
            break;
        case RptType_Event:
            sqlStr.sprintf("insert into RPT_EVT_TAB (ID,TYPE,DESC,TIME,VAL) values (%d,%d,%s,datetime('%04d-%02d-%02d %02d:%02d:%02d'),%d)",
                           m_RptNum[rptType],
                           rptRecord->RptType,
                           rptRecord->RptDesc.toUtf8().data(),
                           rptRecord->RptDateTime.date().year(),
                           rptRecord->RptDateTime.date().month(),
                           rptRecord->RptDateTime.date().day(),
                           rptRecord->RptDateTime.time().hour(),
                           rptRecord->RptDateTime.time().minute(),
                           rptRecord->RptDateTime.time().second(),
                           rptRecord->RptVal);
            break;
        case RptType_Action:
            sqlStr.sprintf("insert into RPT_ACT_TAB (ID,TYPE,DESC,TIME,VAL) values (%d,%d,%s,datetime('%04d-%02d-%02d %02d:%02d:%02d'),%d)",
                           m_RptNum[rptType],
                           rptRecord->RptType,
                           rptRecord->RptDesc.toUtf8().data(),
                           rptRecord->RptDateTime.date().year(),
                           rptRecord->RptDateTime.date().month(),
                           rptRecord->RptDateTime.date().day(),
                           rptRecord->RptDateTime.time().hour(),
                           rptRecord->RptDateTime.time().minute(),
                           rptRecord->RptDateTime.time().second(),
                           rptRecord->RptVal);
            break;
        case RptType_Wave:
            sqlStr.sprintf("insert into RPT_WAVE_TAB (ID,TYPE,DESC,TIME,VAL,FILE) values (%d,%d,%s,datetime('%04d-%02d-%02d %02d:%02d:%02d'),%d,%s)",
                           m_RptNum[rptType],
                           rptRecord->RptType,
                           rptRecord->RptDesc.toUtf8().data(),
                           rptRecord->RptDateTime.date().year(),
                           rptRecord->RptDateTime.date().month(),
                           rptRecord->RptDateTime.date().day(),
                           rptRecord->RptDateTime.time().hour(),
                           rptRecord->RptDateTime.time().minute(),
                           rptRecord->RptDateTime.time().second(),
                           rptRecord->RptVal,
                           rptRecord->RptFile.toUtf8().data());
            break;
        default:
            break;
    }

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
        return ret;
    }

    return 1;
}

int Devreport::RptQuery(eRptType rptType, RptRecord_t* rptRecord)
{
    int ret = -1;
    QString sqlStr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    switch(rptType)
    {
        case RptType_Oper:
            sqlStr.sprintf("select * from RPT_OPT_TAB where ID=%d", m_RptNO[rptType]);
            break;
        case RptType_Event:
            sqlStr.sprintf("select * from RPT_EVT_TAB where ID=%d", m_RptNO[rptType]);
            break;
        case RptType_Action:
            sqlStr.sprintf("select * from RPT_ACT_TAB where ID=%d", m_RptNO[rptType]);
            break;
        case RptType_Wave:
            sqlStr.sprintf("select * from RPT_WAVE_TAB where ID=%d", m_RptNO[rptType]);
            break;
        default:
            break;
    }

    if(!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
        return ret;
    }

    if(query.first())
    {
        m_RptNO[rptType]++;

        rptRecord->RptType = (eRptType)query.value(1).toInt();
        rptRecord->RptDesc = query.value(2).toString();
        rptRecord->RptDateTime = query.value(3).toDateTime();
        rptRecord->RptVal = query.value(4).toInt();
        if(rptRecord->RptType == RptType_Wave)
            rptRecord->RptFile = query.value(5).toString();
    }

    ret = 1;
    return ret;
}

int Devreport::RptGetNum(eRptType rptType)
{
    int ret = -1;
    QString sqlStr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    switch(rptType)
    {
        case RptType_Oper:
            sqlStr.sprintf("select * from RPT_OPT_TAB");
            break;
        case RptType_Event:
            sqlStr.sprintf("select * from RPT_EVT_TAB");
            break;
        case RptType_Action:
            sqlStr.sprintf("select * from RPT_ACT_TAB");
            break;
        case RptType_Wave:
            sqlStr.sprintf("select * from RPT_WAVE_TAB");
            break;
        default:
            break;
    }

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
        return ret;
    }

    m_RptNum[rptType] = query.size();
    ret = (int)m_RptNum[rptType];

    return ret;
}

void Devreport::RptClrNO(eRptType rptType)
{
    m_RptNO[rptType] = 0;
}
