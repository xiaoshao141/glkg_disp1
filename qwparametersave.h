#ifndef QWPARAMETERSAVE_H
#define QWPARAMETERSAVE_H

#include <QDialog>
#include <QSettings>

#define ADDR_HigtWARN 0x4000
#define ADDR_LOWWARN 0x4001
#define ADDR_HUMWARN 0x4002

#define SYS_PATH "D:\\Qt_Dir\\sys.ini"

namespace Ui {
class QWParameterSave;
}

class QWParameterSave : public QDialog
{
    Q_OBJECT

public:
    explicit QWParameterSave(QWidget *parent = nullptr);
    ~QWParameterSave();

    void init();

    int HigtValue=0,LowValue=0,HumValue=0;


private slots:
    void on_btnParameterSave_clicked();


signals:


private:
    Ui::QWParameterSave *ui;

    QSettings *configIni =new QSettings(SYS_PATH,QSettings::IniFormat);
};

#endif // QWPARAMETERSAVE_H
