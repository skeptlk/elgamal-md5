#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/elgamal.h"
#include "core/md5.h"
#include <sstream>


const QVector<int> signPages = {1, 2, 3};
const QVector<int> verifyPages = {4, 5, 6};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stepsWidget->setVisible(false);
    ui->btnNextStep->setVisible(false);

    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);

    connect(ui->btnNextStep, SIGNAL(clicked()), this, SLOT(nextClick()));
    connect(this, &MainWindow::pageChanged, ui->stepsWidget, &StepWidget::setStep);
    connect(ui->btnCreateSignature, SIGNAL(clicked()), this, SLOT(createSignatureClick()));

    connect(ui->btnSignMode, &QPushButton::clicked, this, [=]() {
        ui->stack->setCurrentIndex(signPages[0]);
        ui->stepsWidget->setVisible(true);
        ui->btnNextStep->setVisible(true);
        setWindowTitle("Signing a new message");
        emit pageChanged(0);
        emit pageCountChanged(signPages.size());
    });

    connect(ui->btnVerifyMode, &QPushButton::clicked, this, [=]() {
        ui->stack->setCurrentIndex(verifyPages[0]);
        ui->stepsWidget->setVisible(true);
        ui->btnNextStep->setVisible(true);
        setWindowTitle("Verifying signature");
        emit pageChanged(0);
        emit pageCountChanged(verifyPages.size());
    });

    connect(ui->btnSelectFile, &QPushButton::clicked, this, [=]() {
        fileDialog->setFileMode(QFileDialog::ExistingFile);
        if (fileDialog->exec()) {
            QStringList files = fileDialog->selectedFiles();
            if (files.length() >= 1) {
                QFileInfo file(files[0]);
                ui->leSelectedFile->setText(file.fileName());
            }
        }
    });
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


