#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QFutureWatcher>
#include <QFuture>
#include <QtConcurrent>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QElapsedTimer>
#include <fstream>
#include <sstream>
#include "core/elgamal.h"
#include "core/md5.h"
#include "elgamaladapter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum Mode { MODE_NONE, MODE_SIGN, MODE_VERIFY };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Mode mode = MODE_NONE;
    ElGamalAdapter *elgamalAdapter;
    QString filePath = "";
    QFileDialog *fileDialog;
    QValidator *infNumValidator;
    QFutureWatcher<AsyncResult> signAsyncWatcher;
    QFutureWatcher<bool> verifyAsyncWatcher;
    QElapsedTimer elapsedTimer;
    void showSignResult();
    void showVerifyResult();

public slots:
    void nextClick();
    void prevClick();
    void createSignatureClick();
    void verifySignatureClick();
    void lineEditRejected();
    void lineEditAccepted();
    void onFileSelected();

signals:
    void pageChanged(int i);
    void nextStep();
    void prevStep();
    void pageCountChanged(int i);
};
#endif // MAINWINDOW_H
