#ifndef DEVREPORT_H
#define DEVREPORT_H

#include <QString>
#include <QTime>
#include <QObject>

//#define DEVREPORT_DEBUG_EN    (true)
#if defined(DEVREPORT_DEBUG_EN) && DEVREPORT_DEBUG_EN
#define devreport_debug     qDebug
#else
#define devreport_debug(...)
#endif

/*!
 * @brief 定义报告记录类型
 */
typedef enum
{
    RptType_Oper = 0,   /*!< 操作报告 */

    RptType_Event,      /*!< 事件报告 */

    RptType_Action,     /*!< 动作报告 */

    RptType_Wave,       /*!< 录波报告 */

    RptType_Num         /*!< 报告类型数目 */

} eRptType;


/*!
 * @brief 定义报告管理描述结构
 */
typedef struct
{
    eRptType        RptType;        /*!< 报告记录类型 */

    QString         RptDesc;        /*!< 报告记录描述 */

    QDateTime       RptDateTime;    /*!< 报告记录日期时间 */

    unsigned int    RptVal;         /*!< 报告记录数值 */

    QString         RptFile;        /*!< 录波文件路径（仅录波报告有效） */

} RptRecord_t;


class Devreport : public QObject
{
public:
    Devreport();
    ~Devreport();

public slots:
    int     RptInit(void);

    int     RptClear(eRptType rptType);

    int     RptPutIn(eRptType rptType, RptRecord_t* rptRecord);

    int     RptQuery(eRptType rptType, RptRecord_t* rptRecord);

    int     RptGetNum(eRptType rptType);

    void    RptClrNO(eRptType rptType);

private:
    int     m_RptNO[RptType_Num];

    int     m_RptNum[RptType_Num];
};

#endif // DEVREPORT_H
