#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

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
    QFileDialog* fileDialog;

public slots:
    void nextClick();
    void createSignatureClick();

signals:
    void pageChanged(int i);
    void pageCountChanged(int i);
};
#endif // MAINWINDOW_H
