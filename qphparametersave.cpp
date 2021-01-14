#include "qphparametersave.h"
#include "ui_qphparametersave.h"


QPHParameterSave::QPHParameterSave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QPHParameterSave)
{
    ui->setupUi(this);
    setWindowTitle(QString("隔离开关主导电回路红外测温监测参数设置"));

    OverTemWarn = configIniPhoto->value("Photo/OverTemWarn").toInt();
    HighTemWarn =  configIniPhoto->value("Photo/HighTemWarn").toInt();

    ui->lineOverTem->setText(QString::number(OverTemWarn));
    ui->lineHighTem->setText(QString::number(HighTemWarn));

}

QPHParameterSave::~QPHParameterSave()
{
    delete ui;
    delete configIniPhoto;
}

void QPHParameterSave::on_btnPhotoSetting_clicked()
{
    OverTemWarn = ui->lineOverTem->text().toInt();
    HighTemWarn = ui->lineHighTem->text().toInt();

    configIniPhoto->setValue("Photo/OverTemWarn",OverTemWarn);
    configIniPhoto->setValue("Photo/HighTemWarn",HighTemWarn);

    this->hide();
}
