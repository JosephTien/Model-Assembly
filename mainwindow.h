#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <maintainthread.h>
#include <maintainvar.h>
#include <mainwindow_support.h>

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

    void on_btnDrawCir_clicked();

    void on_btnLoadSkel_clicked();

    void on_btnCutSkel_clicked();

    void on_showSkel_stateChanged(int arg1);

    void on_edgeNum_valueChanged(int arg1);

    void on_btnShell_clicked();

    void on_sig1_stateChanged(int arg1);
\
    void on_btnIso_clicked();

    void on_btnCut_s_clicked();

    void on_btnTest2_clicked();

    void on_btnDecompose_clicked();

    void on_btn_cut_s2_clicked();

    void on_btn_cut_s3_clicked();

    void on_btnPushFace_clicked();

    void on_btnPullFace_clicked();

    void on_btnIso_2_clicked();

    void on_btnDecompose_2_clicked();
private:

    Ui::MainWindow *ui;
    MaintainVar var;
    MainWindow_support support;
    MaintainThread maintainthread;
    void quickpush(int n);
    void quickpull(int n);
    void isolateBody();

};

#endif // MAINWINDOW_H
