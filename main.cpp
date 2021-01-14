#include "mainwindow.h"

#include "modbuspro.h"
#include "sgmdb.h"
#include <QApplication>
//#include <QWSServer>
#include <QFile>
//#include <QtExtSerialPort/qextserialport.h>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QEvent>

#include <QQuickView>
#include <QGuiApplication>
#include <QQmlEngine>

#include "ctemprmtp.h"
#include "ctemmodbus.h"

Devcomm*        g_Devcomm;
Devpdcomm*      g_DevpdComm;
Devoper*        g_Devoper;
Devreport*      g_Devreport;
Devwave*        g_Devwave;
ModbusPro*      g_ModbusPro;
DevMsgMannage*  g_DevMsgMannage;


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initDbs();

    g_Devcomm = new Devcomm();
    g_DevpdComm = new Devpdcomm();
    g_Devoper = new Devoper();
    g_Devreport = new Devreport();
    g_Devwave = new Devwave();
    g_ModbusPro = new ModbusPro();
    g_DevMsgMannage = new DevMsgMannage();

    g_ModbusPro->Init();
    g_Devcomm->Init();
    g_DevpdComm->Init();
    g_Devwave->Init();

    CTempRmtp::initFfmpeg();

    CTempRmtp aCam("192.168.0.23", 1);
    aCam.start();
    CTempRmtp bCam("192.168.0.24", 2);
    bCam.start();
    CTempRmtp cCam("192.168.0.25", 3);
    cCam.start();

    CTemModbus tempModbus;

    MainWindow w;
    //w.show();
    w.showFullScreen();



    return a.exec();
}
