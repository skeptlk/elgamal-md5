#ifndef STEPSWIDGET_H
#define STEPSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>


#define NUMBER_OF_STEP 4

class StepWidget : public QWidget
{
    Q_OBJECT
    int activeStep = 0;
    QPushButton* buttons[NUMBER_OF_STEP];
    QRect* dashes[NUMBER_OF_STEP - 1];

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
