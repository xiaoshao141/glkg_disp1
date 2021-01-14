#include "modbuspro.h"
#include "sgmdb.h"

//#include "mainwnd.h"
#include "mainwindow.h"

ModbusPro::ModbusPro()
{
    modbusDevice = new QModbusRtuSerialSlave(this);

    m_MBIsEnable = false;

    m_MBInputStartAddr = 0;
    m_MBHoldStartAddr = 0;
    m_MBInputRegNum = 0;
    m_MBHoldRegNum = 0;
}

ModbusPro::~ModbusPro()
{
    if(m_MBIsEnable)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
}

void ModbusPro::Init(void)
{
    if(!m_MBIsEnable)
    {
        m_MBInputStartAddr = getMBInputRegStartAddr();
        m_MBHoldStartAddr = getMBHoldRegStartAddr();
        m_MBInputRegNum = getMBInputRegNum();
        m_MBHoldRegNum = getMBHoldRegNum();

        modbus_debug("Modbus Input Reg Start Addr: %d", m_MBInputStartAddr);
        modbus_debug("Modbus Hold Reg Start Addr: %d", m_MBHoldStartAddr);
        modbus_debug("Modbus Input Reg Num: %d", m_MBInputRegNum);
        modbus_debug("Modbus Hold Reg Num: %d", m_MBHoldRegNum);

        QModbusDataUnitMap reg;
        reg.insert(QModbusDataUnit::InputRegisters,
                    { QModbusDataUnit::InputRegisters,
                      (int)m_MBInputStartAddr,
                      (int)m_MBInputRegNum });
        reg.insert(QModbusDataUnit::HoldingRegisters,
                    { QModbusDataUnit::HoldingRegisters,
                      (int)m_MBHoldStartAddr,
                      (int)m_MBHoldRegNum });

        modbusDevice->setMap(reg);
        //为来自其他ModBus客户端的请求设置要映射的已注册映射结构。
        //寄存器值被初始化为零。成功返回true;否则错误。

        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                            MB_SERIAL_NAME);
        //setConnectionParameter
        //将参数的值设置为value。如果参数已经存在，则覆盖之前的值。活动的或正在运行的连接不受这些参数更改的影响。
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                            MB_SERIAL_PARITYBITS);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                            MB_SERIAL_BAUDRATE);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                            MB_SERIAL_DATABITS);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                            MB_SERIAL_STOPBITS);

        modbusDevice->setServerAddress(MB_SLAVE_ADDR);
        //将这个Modbus服务器实例的地址设置为MB_SLAVE_ADDR。

        m_MBIsEnable = modbusDevice->connectDevice();
        if(m_MBIsEnable)
            modbus_debug("[Modbus] serial connected successed \r\n");
        else
            modbus_debug("[Modbus] serial connected errored \r\n");

        connect(modbusDevice, SIGNAL(dataWritten(QModbusDataUnit::RegisterType,int,int)),
                this, SLOT(modbusRegDataChanged(QModbusDataUnit::RegisterType,int,int)));
    }
}

/*!
 * @brief Modbus 输入/保持寄存器点表数据操作部分
 */
unsigned int ModbusPro::getMBInputRegStartAddr(void)
{
    unsigned int reg_start_addr = 0;

    reg_start_addr = MB_INPUT_REG_START_ADDR;

    return reg_start_addr;
}

unsigned int ModbusPro::getMBHoldRegStartAddr(void)
{
    unsigned int reg_start_addr = 0;

    reg_start_addr = MB_HOLD_REG_START_ADDR;

    return reg_start_addr;
}

unsigned int ModbusPro::getMBInputRegNum(void)
{
    unsigned int reg_num = 0;

    if (!g_sgmDb) return 0;
    QSqlQuery query(*g_sgmDb);
    //提供了一种执行和操作SQL语句的方法

    /* @brief Modbus 输入寄存器数据点数量 */
    if (!query.exec("select * from MB_INPUT_REG_TAB"))
    {
        qDebug() << query.lastError();
        return 0;
    }

    reg_num = query.size();
    return reg_num;
}

unsigned int ModbusPro::getMBHoldRegNum(void)
{
    unsigned int reg_num = 0;

    if (!g_sgmDb) return 0;
    QSqlQuery query(*g_sgmDb);

    /* @brief Modbus 保持寄存器数据点数量 */
    if (!query.exec("select * from MB_HOLD_REG_TAB"))
    {
        qDebug() << query.lastError();
        return 0;
    }

    reg_num = query.size();
    return reg_num;
}

unsigned int ModbusPro::getMBInputRegObjAddr(unsigned int reg_addr)
{
    unsigned int obj_addr = 0;
    QString sqlStr;
//QSqlDatabase* g_sgmDb;在"sgmdb.h"里定义
//QSqlDatabase类处理到数据库的连接。g_sgmDb==NULL
    if (!g_sgmDb) return 0;
    QSqlQuery query(*g_sgmDb);
    //用数据库(g_sgmDb)db构造一个QSqlQuery对象。如果db无效，将使用应用程序的默认数据库。

    /* @brief 查询输入寄存器对应内部数据点地址 */
    sqlStr.sprintf("select INNER_ADDR from MB_INPUT_REG_TAB where REG_ADDR=%d", reg_addr);
    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
        return 0;
    }

    if (query.first())
        //first如果可用，检索结果中的第一个记录，并在检索到的记录上定位查询。
        //注意，结果必须处于活动状态，并且isSelect()在调用此函数之前必须返回true，否则它将什么也不做，返回false
        obj_addr = query.value(0).toInt();

    return obj_addr;
}

unsigned int ModbusPro::getMBHoldRegObjAddr(unsigned int reg_addr)
{
    unsigned int obj_addr = 0;
    QString sqlStr;

    if (!g_sgmDb) return 0;
    QSqlQuery query(*g_sgmDb);

    /* @brief 查询保持寄存器对应内部数据点地址 */
    sqlStr.sprintf("select INNER_ADDR from MB_HOLD_REG_TAB where REG_ADDR=%d", reg_addr);
    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
        return 0;
    }

    if (query.first())
        obj_addr = query.value(0).toInt();

    return obj_addr;
}

int ModbusPro::getMBInputRegData(unsigned int reg_addr, unsigned int* reg_data)
{
    int ret = -1;
    QString sqlstr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    /* @brief Modbus 输入寄存器数据 */
    sqlstr.sprintf("select REG_DATA from MB_INPUT_REG_TAB where REG_ADDR==%d", (int)reg_addr);
    if (!query.exec(sqlstr))
    {
        qDebug() << query.lastError();
        return ret;
    }

    if (query.first())
    {
        *reg_data = (unsigned int)query.value(0).toInt();
        ret = 1;
    }

    return ret;
}

int ModbusPro::getMBHoldRegData(unsigned int reg_addr, unsigned int* reg_data)
{
    int ret = -1;
    QString sqlstr;

    if (!g_sgmDb) return ret;
    QSqlQuery query(*g_sgmDb);

    /* @brief Modbus 保持寄存器数据 */
    sqlstr.sprintf("select REG_DATA from MB_HOLD_REG_TAB where REG_ADDR==%d", (int)reg_addr);
    if (!query.exec(sqlstr))
    {
        qDebug() << query.lastError();
        return ret;
    }

    if (query.first())
    {
        *reg_data = (unsigned int)query.value(0).toInt();
        ret = 1;
    }

    return ret;
}

int ModbusPro::setMBHoldRegData(unsigned int reg_addr, unsigned int reg_data)
{
    int ret = -1;
    unsigned int obj_addr = 0;

    ret = (int)modbusDevice->setData(QModbusDataUnit::HoldingRegisters, reg_addr, reg_data);

    obj_addr = getMBHoldRegObjAddr(reg_addr);
    ret = g_Devoper->setDevDBObjData(obj_addr, &reg_data);

    return ret;
}

/*!
 * @brief Modbus 输入/保持寄存器点表更新部分
 */
void ModbusPro::updateRegData(void)
{
    updateInputRegData();

    updateHoldRegData();
}

void ModbusPro::updateInputRegData(void)
{
    int          ret = -1;
    unsigned int obj_addr = 0;
    unsigned int reg_data = 0;

    for(unsigned int reg_addr=m_MBInputStartAddr; reg_addr<m_MBInputRegNum; reg_addr++)
    {
        obj_addr = getMBInputRegObjAddr(reg_addr);
        ret = g_Devoper->getDevDBObjData(obj_addr, &reg_data);
        if(ret >= 0)
        {
            modbusDevice->setData(QModbusDataUnit::InputRegisters, reg_addr, reg_data);
        }
    }
}

void ModbusPro::updateHoldRegData(void)
{
    int          ret = -1;
    unsigned int obj_addr = 0;
    unsigned int reg_data = 0;

    for(unsigned int reg_addr=m_MBHoldStartAddr; reg_addr<m_MBHoldRegNum; reg_addr++)
    {
        obj_addr = getMBHoldRegObjAddr(reg_addr);
        ret = g_Devoper->getDevDBObjData(obj_addr, &reg_data);
        if(ret >= 0)
        {
            modbusDevice->setData(QModbusDataUnit::HoldingRegisters, reg_addr, reg_data);
        }
    }
}

void ModbusPro::updateDBData(void)
{
    unsigned int reg_data = 0;

    /* Update the data of hold reg table to the db. */
    for(unsigned int i=0; i<m_MBHoldRegNum; i++)
    {
        reg_data = getMBHoldRegData(m_MBHoldStartAddr + i, &reg_data);

        setMBHoldRegData(m_MBHoldStartAddr + i, reg_data);
    }
}

void ModbusPro::modbusRegDataChanged(QModbusDataUnit::RegisterType table, int address, int size)
{
    modbus_debug("RegTable: %d \r\n RegAddr: %d \r\n RegSize: %d \r\n",
           table, address, size);

    updateDBData();

    // +++ Update the SGM DEV data in the hold reg table.
}
