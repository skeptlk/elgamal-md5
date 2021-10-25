#ifndef STEPSWIDGET_H
#define STEPSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QRect>

class StepWidget : public QWidget
{
    Q_OBJECT
    const int count = 3;
    int activeStep = 0;
    QPushButton* buttons[3];
    QRect* dashes[3 - 1];

public:
    StepWidget(QWidget *parent = 0);

signals:
    void onStepClick(int i);

public slots:
    void nextStep();
    void prevStep();
    void setStep(int i);

};

#endif // STEPSWIDGET_H
