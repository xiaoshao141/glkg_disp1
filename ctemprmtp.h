#ifndef CTEMPRMTP_H
#define CTEMPRMTP_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QImage>

#define __STDC_CONSTANT_MACROS

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libavformat/version.h>
    #include <libavutil/time.h>
    #include <libavutil/mathematics.h>
}


class CTempRmtp : public QThread
{
    Q_OBJECT
public:
    CTempRmtp();

    CTempRmtp(QString camIp, qint8 camId);

    void startRqstImg();

    static void initFfmpeg();

    void run();
private:
    QString m_camIp;
    qint8  m_camId;
};

extern QImage g_aTempImg;
extern QImage g_bTempImg;
extern QImage g_cTempImg;

#endif // CTEMPRMTP_H
