#ifndef QPHPARAMETERSAVE_H
#define QPHPARAMETERSAVE_H

#include <QDialog>
#include <QSettings>

#define SYS_PATH "D:\\Qt_Dir\\sys.ini"

namespace Ui {
class QPHParameterSave;
}

class QPHParameterSave : public QDialog
{
    Q_OBJECT

public:
    explicit QPHParameterSave(QWidget *parent = nullptr);
    ~QPHParameterSave();

private slots:
    void on_btnPhotoSetting_clicked();

public:
    int OverTemWarn=0,HighTemWarn=0;

private:
    Ui::QPHParameterSave *ui;

    QSettings *configIniPhoto =new QSettings(SYS_PATH,QSettings::IniFormat);
};

#endif // QPHPARAMETERSAVE_H
