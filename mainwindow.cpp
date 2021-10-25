#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/elgamal.h"
#include "core/md5.h"
#include <sstream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnNextStep, SIGNAL(clicked()), this, SLOT(nextClick()));
    connect(this, &MainWindow::pageChanged, ui->stepsWidget, &StepWidget::setStep);
    emit pageChanged(0);
    connect(ui->btnCreateSignature, SIGNAL(clicked()), this, SLOT(createSignatureClick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextClick()
{
    int i = (ui->stack->currentIndex() + 1) % ui->stack->count();
    ui->stack->setCurrentIndex(i);

    emit pageChanged(i);
}

void MainWindow::createSignatureClick()
{
    // collect signature components
    QString p = ui->lePrime->text(),
            alpha = ui->leAlpha->text(),
            z = ui->leZet->text(),
            message = ui->teMessage->toPlainText();

    PrivateKey privateKey = {
        z.toUInt(),
        p.toUInt(),
        alpha.toUInt(),
    };

    auto el = new ElGamal(privateKey);

    std::istringstream ss(message.toStdString());
    uint32_t *hash = md5(&ss);
    InfInt intHash = hash[0];
    intHash *= InfInt(0x100000000);
    intHash += hash[1];
    intHash *= InfInt(0x100000000);
    intHash += hash[2];
    intHash *= InfInt(0x100000000);
    intHash += hash[3];

    SignedMessage sm = el->sign(intHash);

    PublicKey publicKey = el->genPublicKey();

    std::string smtext = "M = " + sm.m.toString() + " (decimal value of hash)\n" +
            "R = " + sm.r.toString() + "\n" +
            "S = " + sm.s.toString() + "\n";

    std::string pktext = "Alpha = " + publicKey.alpha.toString() + "\n" +
            "Beta = " + publicKey.beta.toString() + "\n" +
            "Prime = " + publicKey.p.toString() + "\n";

    ui->labelResult_1->setText(QString::fromStdString(smtext));
    ui->labelResult_2->setText(QString::fromStdString(pktext));

}


