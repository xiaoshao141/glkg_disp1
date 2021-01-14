#ifndef QWPHOTOES_H
#define QWPHOTOES_H

#include <QDialog>
#include <QTimer>

#include "qphparametersave.h"
#include "qphanalysis.h"

namespace Ui {
class QWPhotoes;
}

class QWPhotoes : public QDialog
{
    Q_OBJECT

public:
    explicit QWPhotoes(QWidget *parent = nullptr);
    ~QWPhotoes();

    void TemCompare();

    QPHParameterSave *myPHParameter = new QPHParameterSave(this);
    QPHAnalysis *myPHAnalysis = new QPHAnalysis(this);

private slots:
    void on_btnParameterSetting_clicked();

    void on_btnAnalsis_clicked();

    void timerOut();


private:
    Ui::QWPhotoes *ui;

    QTimer* m_pTimer;
};

#endif // QWPHOTOES_H
