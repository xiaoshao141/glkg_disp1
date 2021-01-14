#ifndef MODBUSPRO_H
#define MODBUSPRO_H

#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QModbusServer>
#include <QModbusRtuSerialSlave>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>



#define MB_SERIAL_NAME          "ttyS5"
#define MB_SERIAL_BAUDRATE      (QSerialPort::Baud115200)
#define MB_SERIAL_STOPBITS      (QSerialPort::OneStop)
#define MB_SERIAL_DATABITS      (QSerialPort::Data8)
#define MB_SERIAL_PARITYBITS    (QSerialPort::NoParity)

#define MB_SLAVE_ADDR           (0x01)

#define MB_INPUT_REG_START_ADDR (0x01)
#define MB_HOLD_REG_START_ADDR  (0x01)

//#define MODBUS_DEBUG_EN    (true)
#if defined(MODBUS_DEBUG_EN) && MODBUS_DEBUG_EN
#define modbus_debug     qDebug
#else
#define modbus_debug(...)
#endif

class ModbusPro : public QObject
{
public:
    ModbusPro();
    ~ModbusPro();

    void Init(void);

public:
    void updateRegData(void);

    void updateDBData(void);

public slots:
    void modbusRegDataChanged(QModbusDataUnit::RegisterType table, int address, int size);

private:

    void updateInputRegData(void);
    void updateHoldRegData(void);

    unsigned int getMBInputRegStartAddr(void);
    unsigned int getMBHoldRegStartAddr(void);

    unsigned int getMBInputRegNum(void);
    unsigned int getMBHoldRegNum(void);

    unsigned int getMBInputRegObjAddr(unsigned int reg_addr);
    unsigned int getMBHoldRegObjAddr(unsigned int reg_addr);

    int getMBInputRegData(unsigned int reg_addr, unsigned int* reg_data);
    int getMBHoldRegData(unsigned int reg_addr, unsigned int* reg_data);

    int setMBHoldRegData(unsigned int reg_addr, unsigned int reg_data);

private:
    QModbusServer*  modbusDevice;
    bool            m_MBIsEnable;
    unsigned int    m_MBInputStartAddr;
    unsigned int    m_MBHoldStartAddr;
    unsigned int    m_MBInputRegNum;
    unsigned int    m_MBHoldRegNum;

};

#endif // MODBUSPRO_H
