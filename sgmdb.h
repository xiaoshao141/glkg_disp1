#ifndef SGMDB_H
#define SGMDB_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#define DEV_DB_DIR    "E:\\Qt_Dir\\DB\\"

extern QSqlDatabase* g_realdb;
extern QSqlDatabase* g_sgmDb;

bool initDbs(void);

#endif // SGMDB_H
