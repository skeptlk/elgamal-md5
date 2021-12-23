#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QFutureWatcher>
#include "core/elgamal.h"
#include "core/md5.h"

struct AsyncResult {
    PublicKey pk;
    SignedMessage sm;
};

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
    QString fileName = "";
    QFileDialog* fileDialog;
    QFutureWatcher<AsyncResult> dataWatcher;
    void showResult();

public slots:
    void nextClick();
    void prevClick();
    void createSignatureClick();

signals:
    void pageChanged(int i);
    void nextStep();
    void prevStep();
    void pageCountChanged(int i);
};
#endif // MAINWINDOW_H
