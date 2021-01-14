#ifndef CTEMMODBUS_H
#define CTEMMODBUS_H

#include <QObject>
#include <QTimer>
#include <QTcpSocket>

class CTemModbus : public QObject
{
    Q_OBJECT
public:
    CTemModbus();

    CTemModbus(QString ip);

public slots:
    void CtrlMask(int);

    void rsvMsg(void);

    void rqstMsg(void);


private:
    QTcpSocket* m_pSkt;

    QTimer* m_pTimer;

    char  dataP;

    QString m_ip;
};

extern qreal aTemp;
extern qreal bTemp;
extern qreal cTemp;

#endif // CTEMMODBUS_H
