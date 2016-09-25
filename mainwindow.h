#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iglmachine.h>
#include <cgaltool.h>

#include<modelmanager.h>
#include<viewmanager.h>
#include<modelmanager.h>
#include "ui_mainwindow.h"

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
    void on_btnLoad_clicked();

    void mousePressEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *);

    void keyReleaseEvent(QKeyEvent *event);

    void on_btnTest_clicked();

    void on_edition_stateChanged(int arg1);

    void on_selection_stateChanged(int arg1);

    void on_btnNormalize_clicked();

    void on_btnCurv_clicked();

    void on_btnDetour_clicked();

    void on_cmbbSelection_currentIndexChanged(int index);

    void on_btnReset_clicked();

    void on_showPlate_stateChanged(int arg1);

    void on_btnCut_clicked();

    void on_btnOutput_clicked();

    void on_cmbbObject_currentIndexChanged(int index);

    void on_btnPushConnector_clicked();

    void on_btnPullConnector_clicked();

    void on_btnPutConnector_clicked();

    void on_btnSwell_clicked();

    void on_btnDig_clicked();

private:

    Ui::MainWindow *ui;
    IglMachine iglMachine;
    CGALTool cgaltool;
    int tarObj = 0;
    int assistPlaneIdx = 0;

    bool quickmode=true;
    float normalizeVal = 100.0f;
    int totalconnectType=2;
    int connectType = 0;
    bool connectPuted = false;
    int connectTarNum = 0;

    ModelManager * getModel(int tar);
    void putStdModel(char * name, QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate);
    void applyCSG(char c,int er, int ee);
};

#endif // MAINWINDOW_H
