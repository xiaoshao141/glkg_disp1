#ifndef QPHANALYSIS_H
#define QPHANALYSIS_H

#include <QDialog>

namespace Ui {
class QPHAnalysis;
}

class QPHAnalysis : public QDialog
{
    Q_OBJECT

public:
    explicit QPHAnalysis(QWidget *parent = nullptr);
    ~QPHAnalysis();

    void aTemNormal();
    void aOverTemWarn();
    void aHighTemWarn();
    void bTemNormal();
    void bOverTemWarn();
    void bHighTemWarn();
    void cTemNormal();
    void cOverTemWarn();
    void cHighTemWarn();

private:
    Ui::QPHAnalysis *ui;
};

#endif // QPHANALYSIS_H
