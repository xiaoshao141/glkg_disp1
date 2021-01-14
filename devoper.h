#ifndef DEVOPER_H
#define DEVOPER_H

#include <QObject>


//#define DEVOPER_DEBUG_EN    (true)
#if defined(DEVOPER_DEBUG_EN) && DEVOPER_DEBUG_EN
#define devoper_debug     qDebug
#else
#define devoper_debug(...)
#endif

class Devoper : public QObject
{
public:
    Devoper();
    ~Devoper();

public:
    int getDevObjDesc(unsigned int obj_addr,
                      QString* obj_desc);

public slots:
    int getDevDBObjData(unsigned int obj_addr,
                        void* obj_data);
    int setDevDBObjData(unsigned int obj_addr,
                        void* obj_data);

private:

};

#endif // DEVOPER_H
