#include "stepwidget.h"
#include <QApplication>

const QString normalStyle = "QPushButton {"\
                            "margin: 4px;"\
                            "margin-right: 80px;"\
                            "margin-left: 40px;"\
                            "color: #29e7cd;"\
                            "background: #401F3E;"\
                            "border: 2px solid #29e7cd;"\
                            "font: 400 15pt 'Ubuntu';"\
                            "border-radius: 22px;"\
                            "height: 40px;"\
                            "width: 40px;"\
                        "}";
const QString hoverStyle = "QPushButton:hover {"\
                            "background: #29e7cd;"\
                            "color: #401F3E;"\
                           "}";
const QString activeStyle = "QPushButton {"\
                            "background: #29e7cd;"\
                            "color: #401F3E;"\
                            "}";

StepWidget::StepWidget(QWidget *parent) :
    QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);

    for (int i = 0; i < 3; i++) {
        buttons[i] = new QPushButton(QString::number(i));
        QString style = normalStyle + hoverStyle;
        buttons[i]->setAccessibleName("");
        buttons[i]->setStyleSheet(style);
        buttons[i]->setCursor(Qt::PointingHandCursor);
        layout->addWidget(buttons[i]);
        layout->setSpacing(0);
        layout->setMargin(0);
    }
}

void StepWidget::nextStep()
{
    setStep((activeStep + 1) % NUMBER_OF_STEP);
}

void StepWidget::prevStep()
{
    setStep((activeStep - 1 + NUMBER_OF_STEP) % NUMBER_OF_STEP);
}

void StepWidget::setStep(int i)
{
    buttons[activeStep]->setStyleSheet(normalStyle + hoverStyle);
    buttons[i]->setStyleSheet(normalStyle + activeStyle + hoverStyle);
    activeStep = i;
}




