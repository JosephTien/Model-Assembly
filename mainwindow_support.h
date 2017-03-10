#ifndef MAINWINDOW_SUPPORT_H
#define MAINWINDOW_SUPPORT_H
#include "ui_mainwindow.h"
#include <iglmachine.h>
#include <cgaltool.h>
#include <viewmanager.h>
#include <modelmanager.h>
#include <maintainvar.h>

class MainWindow_support{
public:
    MainWindow_support(){}
    Ui::MainWindow * ui;
    MaintainVar * var;
    IglMachine iglMachine;
    CGALTool cgaltool;
    MainWindow_support(Ui::MainWindow * ui, MaintainVar * var){
        this->ui = ui;
        this->var = var;
    }
    ModelManager * getModel(int tar);
    void putStdModel(const char * name, QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate);
    void applyCSG(char c,int er, int ee);
    void drawCircleOnPlane();
    void putskelass();
    void deleteSkelSup();
    void stateInit();
    void loadSkelSup();
    void fill(int tar);

};

#endif // MAINWINDOW_SUPPORT_H
