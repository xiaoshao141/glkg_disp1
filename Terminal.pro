QT       += core gui
QT       += widgets printsupport

QT       += core gui sql svg xml
QT       += qml quick
QT       += serialbus serialport
QT       += network
QT       += printsupport

QT       += widgets

RC_ICONS = logo.ico

static{
QTPLUGIN += qtvirtualkeyboardplugin
QT       += svg
}

INCLUDEPATH += /root/workspace/allwinner/A40i/bsp/lichee/buildroot-201611/dl/qt-everywhere-opensource-src-5.9.0/qtbase/include/QtANGLE
DEPENDPATH += /root/workspace/allwinner/A40i/bsp/lichee/buildroot-201611/dl/qt-everywhere-opensource-src-5.9.0/qtbase/include/QtANGLE

INCLUDEPATH += $$PWD/ffmpeg-3.3.3-win32-dev/include

LIBS += $$PWD/ffmpeg-3.3.3-win32-dev/lib/libavcodec.dll.a\
        $$PWD/ffmpeg-3.3.3-win32-dev/lib/libavdevice.dll.a\
        $$PWD/ffmpeg-3.3.3-win32-dev/lib/libavfilter.dll.a\
        $$PWD/ffmpeg-3.3.3-win32-dev/lib/libavformat.dll.a\
        $$PWD/ffmpeg-3.3.3-win32-dev/lib/libavutil.dll.a\
        $$PWD/ffmpeg-3.3.3-win32-dev/lib/libswresample.dll.a\
        $$PWD/ffmpeg-3.3.3-win32-dev/lib/libswscale.dll.a\
        $$PWD/ffmpeg-3.3.3-win32-dev/lib/libpostproc.dll.a

LIBS += -L$$PWD/ffmpeg-3.3.3-win32-shared/bin

QT += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    callout.cpp \
    comanalysis.cpp \
    ctemmodbus.cpp \
    ctemprmtp.cpp \
    dashboard1.cpp \
    devcomm.cpp \
    devmsgmannage.cpp \
    devoper.cpp \
    devpdcomm.cpp \
    devreport.cpp \
    devwave.cpp \
    main.cpp \
    mainwindow.cpp \
    modbuspro.cpp \
    mouse1.cpp \
    mouse2.cpp \
    mouse3.cpp \
    mouse4.cpp \
    qcustomplot.cpp \
    qphanalysis.cpp \
    qphparametersave.cpp \
    qwanalysis.cpp \
    qwparameterinf.cpp \
    qwparametersave.cpp \
    qwphotoes.cpp \
    setdata.cpp \
    sgmdb.cpp \
    torquedetection.cpp \
    qchartmouseevent.cpp

HEADERS += \
    callout.h \
    comanalysis.h \
    ctemmodbus.h \
    ctemprmtp.h \
    dashboard1.h \
    devcomm.h \
    devmsgmannage.h \
    devoper.h \
    devpdcomm.h \
    devreport.h \
    devwave.h \
    mainwindow.h \
    modbuspro.h \
    mouse1.h \
    mouse2.h \
    mouse3.h \
    mouse4.h \
    qcustomplot.h \
    qphanalysis.h \
    qphparametersave.h \
    qwanalysis.h \
    qwparameterinf.h \
    qwparametersave.h \
    qwphotoes.h \
    setdata.h \
    sgmdb.h \
    torquedetection.h \
    qchartmouseevent.h

FORMS += \
    comanalysis.ui \
    dashboard1.ui \
    mainwindow.ui \
    qphanalysis.ui \
    qphparametersave.ui \
    qwanalysis.ui \
    qwparameterinf.ui \
    qwparametersave.ui \
    qwphotoes.ui \
    setdata.ui \
    torquedetection.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Terminal.pro.user

RESOURCES += \
    res.qrc \
    res.qrc \
    res.qrc \
    res.qrc
