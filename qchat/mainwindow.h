#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QPlainTextEdit>

#include "aboutwindow.h"
#include "networkfile.h"

class networkFile;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_returnSend_text();

    void on_clearScreen_click();

    void disNetConf();

    void nConnect();

    void nStopConnect();

    void addText(QString value1);

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    AboutWindow * w2;
    networkFile * net;
};

#endif // MAINWINDOW_H
