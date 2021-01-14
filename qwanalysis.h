#ifndef QWANALYSIS_H
#define QWANALYSIS_H

#include <QDialog>

namespace Ui {
class QWAnalysis;
}

class QWAnalysis : public QDialog
{
    Q_OBJECT

public:
    explicit QWAnalysis(QWidget *parent = nullptr);
    ~QWAnalysis();

    void TemperatureNormal();
    void TemperatureHight();
    void TemperatureLow();

    void HumidityNormal();
    void HumidityHight();

private:
    Ui::QWAnalysis *ui;
};

#endif // QWANALYSIS_H
