#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <maintainthread.h>
#include <maintainvar.h>
#include <mainwindow_support.h>
#include <geometry.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow * getUi(){return ui;}

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

    void on_btn_cut_s4_clicked();

    void on_btnPushFace_clicked();

    void on_btnPullFace_clicked();

    void on_btnIso_2_clicked();

    void on_btnDecompose_2_clicked();

    void on_btnTest3_clicked();

    void on_btnForceCut_clicked();

    void on_btnDispPieces_clicked();

    void on_btnExclude_clicked();

    void on_btnLoadHead_clicked();

    void on_btnFixPos_clicked();

    void on_btnArea_clicked();

    void on_btnArea_2_clicked();

    void on_btnMergeAll_clicked();

    void on_btn_cut_s5_clicked();

    void on_btnLoad_Manu_clicked();

    void on_btnSpiral_clicked();

    void on_btnKinect_clicked();

    void on_btnLoadAll_clicked();

    void on_btnCutAll_clicked();

    void on_btnOutputAll_clicked();

    void on_btnFixPos2_clicked();

    void on_btnFixPos3_clicked();

    void on_InterSecA_B_clicked();

    void on_btn_cut_s6_clicked();

    void on_btnAddPlate_clicked();

    void on_sldTranslate_valueChanged(int value);

    void on_sldTile_valueChanged(int value);

    void on_sldRotate_valueChanged(int value);

    void on_sldR_valueChanged(int value);

    void on_pushButton_clicked();

    void on_btnOpt_clicked();

    void on_btnOpt2_clicked();

    void on_btnOpt3_clicked();

    void on_btnArgrst_clicked();

    void on_btnApplyOpt_clicked();

    void on_btnApplyCut_clicked();

    void on_sbTwistAngle_editingFinished();

    void on_btnOptAll_clicked();

    void on_btnPrintRot_clicked();

    void on_btnQuickDone_clicked();

private:

    Ui::MainWindow *ui;
    MaintainVar var;
    MainWindow_support support;
    MaintainThread maintainthread;
    void quickpush(int n);
    void quickpull(int n);
    void isolateBody();
    void putdetail();
    void usedetails();
    void usedetails2();
    void usedetails3();
    void usedetails4();
    void usedetails5();
    void onConnChange();
    void addConnTar(float r, float r2, QVector3D c, QVector3D n1, QVector3D n2, int tar1, int tar2);
    /************************/
    std::vector<float> ss;
    std::vector<float> rs;
    std::vector<float> r2s;
    std::vector<QVector3D> cs;
    std::vector<QVector3D> ns;
    std::vector<QVector3D> n2s;
    std::vector<int> tar1s;
    std::vector<int> tar2s;
    std::vector<int> connTypes;
    std::vector<bool> reverses;
    QVector3D limbNorm[10];
    float radiis[15];
    /************************/
    bool autofix = false;
    bool drawass = true;
    bool loaded=false;
    bool ctrlable = false;
    /************************/
    QVector3D curC;
    QVector3D curPilarC;
    QVector3D curPlateN;
    QVector3D curPilarN;
    float curPlateRotVal;
    float curPlateTileVal;
    float curPlateR;
    float curPilarR;
    bool curPilarnValid = false;
    float curTwistAngle = 90;
    float curAreaRate;
    float curAngleCal;
    /************************/
    int connType = 0;
    float curArea;
    //float TileLimit = 15.0f;
    float TileLimit = 5.0f;
    float acceptableRange = 25.0f;
    bool fastOptMode = true;
    int HDiv = 30;
    /************************/
};

#endif // MAINWINDOW_H
