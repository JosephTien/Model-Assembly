#ifndef MAINTAINTHREAD_H
#define MAINTAINTHREAD_H
#include <QThread>
#include <QDebug>
#include "mainwindow_support.h"
#include "ui_mainwindow.h"
#include "maintainvar.h"
#include "pch.h"

class MaintainThread : public QThread {
public:
    MaintainThread(){}
    Ui::MainWindow *ui;
    MaintainVar *var;
    void setupUi(Ui::MainWindow * ui);
    void setupVar(MaintainVar * var);
public slots:
    void run();

};

#endif // MAINTAINTHREAD_H
