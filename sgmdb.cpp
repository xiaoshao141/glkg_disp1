#include "sgmdb.h"
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>

QSqlDatabase* g_realdb = NULL;

QSqlDatabase* g_sgmDb = NULL;

QSqlDatabase realDb;

QSqlDatabase sgmDb;


bool initDbs(void)
{
    QDir    dir;

    realDb = QSqlDatabase::addDatabase("QSQLITE", "realDb");

    realDb.setDatabaseName(":memory:");
    if (!realDb.open())
    {
        qDebug() << "Open real values db failed";
        return false;
    }
    else
    {
        g_realdb = &realDb;
        qDebug() << "Open real values db sucess";
    }

    sgmDb = QSqlDatabase::addDatabase("QSQLITE", "sgmDb");
    //使用驱动程序类型和连接名称connectionName将数据库添加到数据库连接列表中。
    //如果已经存在一个名为connectionName的数据库连接，则删除该连接。

    dir.setPath(DEV_DB_DIR);
    if(!dir.exists())
        dir.mkpath(DEV_DB_DIR);

    QString fileName = DEV_DB_DIR;
    fileName += "term.db3";
    sgmDb.setDatabaseName(fileName);

    if (!sgmDb.open())
    {
        qDebug() << "Error: Failed to connect database." << sgmDb.lastError();
        return false;
    }
    else
    {
        g_sgmDb = &sgmDb;
        qDebug() << "Succeed to connect database." ;
    }

    return true;
}
