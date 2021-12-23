#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFuture>
#include <QtConcurrent>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include <QRegExpValidator>


const QVector<int> signPages = {1, 2, 3};
const QVector<int> verifyPages = {4, 5, 6};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stepsWidget->setVisible(false);
    ui->btnNextStep->setVisible(false);
    ui->btnPrevStep->setVisible(false);

    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);

    connect(ui->btnNextStep, SIGNAL(clicked()), this, SLOT(nextClick()));
    connect(ui->btnPrevStep, SIGNAL(clicked()), this, SLOT(prevClick()));
    connect(this, &MainWindow::pageChanged, ui->stepsWidget, &StepWidget::setStep);
    connect(this, &MainWindow::nextStep, ui->stepsWidget, &StepWidget::nextStep);
    connect(this, &MainWindow::prevStep, ui->stepsWidget, &StepWidget::prevStep);
    connect(ui->btnCreateSignature, SIGNAL(clicked()), this, SLOT(createSignatureClick()));


    connect(ui->btnSignMode, &QPushButton::clicked, this, [=]() {
        ui->stack->setCurrentIndex(signPages[0]);
        ui->stepsWidget->setVisible(true);
        ui->btnNextStep->setVisible(true);
        ui->btnPrevStep->setVisible(true);
        setWindowTitle("Signing a new message");
        emit pageChanged(0);
        emit pageCountChanged(signPages.size());
    });

    connect(ui->btnVerifyMode, &QPushButton::clicked, this, [=]() {
        ui->stack->setCurrentIndex(verifyPages[0]);
        ui->stepsWidget->setVisible(true);
        ui->btnNextStep->setVisible(true);
        ui->btnPrevStep->setVisible(true);
        setWindowTitle("Verifying signature");
        emit pageChanged(0);
        emit pageCountChanged(verifyPages.size());
    });

    // todo: btnSelectFile_2
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

    // connect all inputs to validators
    QList<QString> leNames {"leAlpha", "lePrime", "leZet", "leR", "leS", "leAlpha_2", "leY"};
    QRegExp rx("\\d+"); // any sequence of digits
    QValidator *infNumValidator = new QRegExpValidator(rx, this);

    for (const QString &name : leNames) {
        QLineEdit* lineEdit = findChild<QLineEdit*>(name);
        QLabel* errorLabel = findChild<QLabel*>(name + "_invalid");
        lineEdit->setValidator(infNumValidator);
        // handle end of user input (focus lost?)
        connect(lineEdit, &QLineEdit::inputRejected, this, &MainWindow::lineEditEditingFinished);
        // hide error message
        if (errorLabel) {
            errorLabel->hide();
        }
    }


    connect(&dataWatcher, &QFutureWatcher<AsyncResult>::finished, this, &MainWindow::showResult);

    ui->loadingSpinner->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextClick()
{
    int i = (ui->stack->currentIndex() + 1) % ui->stack->count();
    ui->stack->setCurrentIndex(i);
    emit nextStep();
}

void MainWindow::prevClick()
{
    int i = (ui->stack->currentIndex() - 1) % ui->stack->count();
    ui->stack->setCurrentIndex(i);
    emit prevStep();
}

void MainWindow::lineEditEditingFinished()
{
    QObject* obj = sender();
    QString name = obj->objectName();
    QLabel* errorMessage = findChild<QLabel*>(name + "_invalid");

    if (errorMessage) {
        errorMessage->show();
    }
}

extern AsyncResult asyncSign(
        std::string message,
        std::string filename,
        PrivateKey<InfInt> privateKey
) {
    auto el = new ElGamal<InfInt>(privateKey);

    std::istream* inputStream;
    if (message.length() > 0) {
        std::ifstream file(filename);
        inputStream = &file;
    }
    else {
        std::istringstream ss(message);
        inputStream = &ss;
    }

    uint32_t *hash = md5(inputStream);

    InfInt intHash = hash[0];
    intHash *= InfInt(0x100000000);
    intHash += hash[1];
    intHash *= InfInt(0x100000000);
    intHash += hash[2];
    intHash *= InfInt(0x100000000);
    intHash += hash[3];

    SignedMessage<InfInt> sm = el->sign(intHash);

    PublicKey<InfInt> publicKey = el->genPublicKey();

    AsyncResult res;
    res.pk = publicKey;
    res.sm = sm;
    return res;
}


void MainWindow::createSignatureClick()
{
    ui->loadingSpinner->setVisible(true);

    // collect signature components
    QString p = ui->lePrime->text(),
            alpha = ui->leAlpha->text(),
            z = ui->leZet->text(),
            message = ui->teMessage->toPlainText();

    if (fileName.size() > 0) {
        std::ifstream file(fileName.toStdString());
        if(!file.is_open()) {
            QMessageBox messageBox;
            messageBox.critical(0, "Error", "Can't open file!");
            messageBox.setFixedSize(500,200);
            messageBox.show();
            return;
        }
        file.close();
    }

    PrivateKey<InfInt> privateKey = {
        z.toUInt(),
        p.toUInt(),
        alpha.toUInt(),
    };

    QFuture<AsyncResult> f = QtConcurrent::run(asyncSign, message.toStdString(), fileName.toStdString(), privateKey);
    dataWatcher.setFuture(f);

    f.begin();
}

void MainWindow::showResult()
{
    AsyncResult res = dataWatcher.result();
    std::string smtext = "M = " + res.sm.m.toString() + " (decimal value of hash)\n" +
            "R = " + res.sm.r.toString() + "\n" +
            "S = " + res.sm.s.toString() + "\n";

    std::string pktext = "Alpha = " + res.pk.alpha.toString() + "\n" +
            "Beta = " + res.pk.beta.toString() + "\n" +
            "Prime = " + res.pk.p.toString() + "\n";

    ui->labelResult_1->setText(QString::fromStdString(smtext));
    ui->labelResult_2->setText(QString::fromStdString(pktext));
    ui->loadingSpinner->setVisible(false);
}
