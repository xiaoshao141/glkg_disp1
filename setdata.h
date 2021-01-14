#ifndef SETDATA_H
#define SETDATA_H

#include <QDialog>

namespace Ui {
class SetData;
}

class SetData : public QDialog
{
    Q_OBJECT

public:
    explicit SetData(QWidget *parent = 0);
    ~SetData();

    int setTorMax=0,setAngMax=0,setCurMax=0,
        setTorMin=0, setAngMin=0,setCurMin=0;
    bool isOk;
    bool ok;

signals:
    void MaxTorChange();

private slots:
    void on_btPreservation_clicked();

private:
    Ui::SetData *ui;
};

#endif // SETDATA_H
