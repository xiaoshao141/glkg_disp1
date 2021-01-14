#include "devoper.h"
#include "sgmdb.h"

#include <QString>
#include <QDebug>


Devoper::Devoper()
{

}

Devoper::~Devoper()
{

}

int Devoper::getDevObjDesc(unsigned int obj_addr,
                           QString* obj_desc)
{
    int     ret = -1;
    QString sqlstr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    /* @brief 查询设备参数数据点数据 */
    if(obj_addr>=0x1000 && obj_addr<0x2000)
        sqlstr.sprintf("select DESC from YX_TAB where ADDR==%d", (int)obj_addr);
    else if(obj_addr>=0x2000 && obj_addr<0x3000)
        sqlstr.sprintf("select DESC from YC_TAB where ADDR==%d", (int)obj_addr);
    else if(obj_addr>=0x4000 && obj_addr<0x5000)
        sqlstr.sprintf("select DESC from YT_TAB where ADDR==%d", (int)obj_addr);

    if(!query.exec(sqlstr))
    {
        qDebug() << sqlstr;
        qDebug() << query.lastError();
    }
    else
    {
        if(query.first())
        {
            *obj_desc = query.value(0).toString();
            ret = 1;
        }
    }

    return ret;
}

int Devoper::getDevDBObjData(unsigned int obj_addr,
                             void* obj_data)
{
    int ret = -1;
    QString sqlstr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    /* @brief 查询设备参数数据点数据 */
    if(obj_addr>=0x1000 && obj_addr<0x2000)
        sqlstr.sprintf("select VAL from YX_TAB where ADDR==%d", (int)obj_addr);
    else if(obj_addr>=0x2000 && obj_addr<0x3000)
        sqlstr.sprintf("select VAL from YC_TAB where ADDR==%d", (int)obj_addr);
    else if(obj_addr>=0x4000 && obj_addr<0x5000)
        sqlstr.sprintf("select VAL from YT_TAB where ADDR==%d", (int)obj_addr);

    if (!query.exec(sqlstr))
    {
        qDebug() << sqlstr;
        qDebug() << query.lastError();
        return ret;
    }

    if (query.first())
    {
        *(float *)obj_data = query.value(0).toFloat();
        ret = 1;
    }

    return ret;
}

int Devoper::setDevDBObjData(unsigned int obj_addr,
                             void* obj_data)
{
    int ret = -1;
    QString sqlstr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    /* @brief 设置设备参数数据点数据 */
    if(obj_addr>=0x1000 && obj_addr<0x2000)
        sqlstr.sprintf("update YX_TAB set VAL=%f where ADDR==%d",
                       *(float *)obj_data, (int)obj_addr);
    else if(obj_addr>=0x2000 && obj_addr<0x3000)
        sqlstr.sprintf("update YC_TAB set VAL=%f where ADDR==%d",
                       *(float *)obj_data, (int)obj_addr);
    else if(obj_addr>=0x4000 && obj_addr<0x5000)
        sqlstr.sprintf("update YT_TAB set VAL=%f where ADDR==%d",
                       *(float *)obj_data, (int)obj_addr);

    if (!query.exec(sqlstr))
    {
        qDebug() << sqlstr;
        qDebug() << query.lastError();
        return ret;
    }

    return ret;
}
