#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    support = MainWindow_support(ui, &var);
    maintainthread.setupVar(&var);
    maintainthread.setupUi(ui);
    maintainthread.start();
    ui->cmbbSelection->addItem(tr("point 1"));
    ui->cmbbSelection->addItem(tr("point 2"));
    ui->cmbbSelection->addItem(tr("point 3"));
}

MainWindow::~MainWindow()
{
    maintainthread.exit();
    delete ui;
}
void MainWindow::on_btnLoad_clicked()
{
    support.stateInit();
    if(var.quickmode){
        //std::string path="../testobj/bunny.obj";
        //std::string path = "./bunny.obj";
        std::string path="./body.obj";
        //std::string path="./cube.obj";
        for(int i=ui->glMain->getTarnum()-1;i>=0;i--){
            ui->glMain->deleteTar(i);
        }
        ui->glMain->load(path.data());

    }else{
        std::string path = QFileDialog::getOpenFileName(this, tr("Load obj"), "..", tr("Object Files(*.obj)")).toStdString();
        if(path.length() == 0) {
                QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        } else {
                //QMessageBox::information(NULL, tr("Path"),tr(path.data()));
                for(int i=ui->glMain->getTarnum()-1;i>=0;i--){
                    ui->glMain->deleteTar(i);
                }
                QMessageBox qmb;
                qmb.setStandardButtons(NULL);
                qmb.setText(tr("                     "));
                qmb.setWindowTitle(tr("Loading......."));
                qmb.show();
                ui->glMain->load(path.data());
                qmb.close();
        }
    }
    ui->glMain->viewMgr->Reset();
    ui->glMain->setVis(0,1);
    ui->glMain->reBuffer(0);
    var.tarObj = 0;
    char str[10];
    sprintf(str,"Object %d",ui->glMain->getTarnum());
    for(int i=ui->cmbbSelection->count()-1;i>=0;i--){
        ui->cmbbObject->removeItem(i);
    }
    ui->cmbbObject->addItem(QString(str));
    this->setFocus();
}
void MainWindow::mousePressEvent(QMouseEvent *){
    this->setFocus();
}
void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_1){
        ui->selection->setChecked(false);
    }
    else if(event->key() == Qt::Key_2){
        ui->selection->setChecked(false);
    }
    else if(event->key() == Qt::Key_3){
        ui->selection->setChecked(false);
    }
    ui->glMain->keyReleaseEvent(event);
    this->setFocus();
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Space){
        ui->edition->toggle();
    }
    else if(event->key() == Qt::Key_Control){
        ui->selection->toggle();
    }
    else if(event->key() == Qt::Key_1){
        ui->cmbbSelection->setCurrentIndex(0);
        ui->selection->setChecked(true);
    }
    else if(event->key() == Qt::Key_2){
        ui->cmbbSelection->setCurrentIndex(1);
        ui->selection->setChecked(true);
    }
    else if(event->key() == Qt::Key_3){
        ui->cmbbSelection->setCurrentIndex(2);
        ui->selection->setChecked(true);
    }else if(event->key() == Qt::Key_E){
        ui->cmbbObject->setCurrentIndex((var.tarObj+ui->cmbbObject->count()-1)%ui->cmbbObject->count());
    }else if(event->key() == Qt::Key_D){
        ui->cmbbObject->setCurrentIndex((var.tarObj+1)%ui->cmbbObject->count());
    }
    ui->glMain->keyPressEvent(event);
    this->setFocus();
}

void MainWindow::on_btnTest_clicked()
{
    //ui->glMain->generateTest();
    //ui->glMain->generateDisc(QVector3D(0,0,0),QVector3D(0,0,1),100.0f, 0);
    ui->btnLoad->click();
    ui->btnLoadSkel->click();
    ui->btnCutSkel->click();
    ui->btnCutSkel->click();
    ui->btnCutSkel->click();
    ui->btnDetour->click();
    ui->btnCut->click();
    ui->cmbbObject->setCurrentIndex(1);
    //ui->btnDrawCir->click();
    this->setFocus();
}

void MainWindow::on_edition_stateChanged(int arg1)
{
    if(arg1 == 0){
        ui->glMain->viewMgr->setControlTar(-1);
        ui->glMain->viewMgr->setControlType(0);
    }else{
         ui->glMain->viewMgr->setControlTar(var.tarObj);
         ui->glMain->viewMgr->setControlType(1);
         for(int i=ui->glMain->getTarnum_ass()-1;i>=0;i--)ui->glMain->deleteTar_ass(i);
         support.getModel(var.tarObj)->selecIdxs.clear();
         support.getModel(var.tarObj)->selecPoints.clear();
         support.getModel(var.tarObj)->loadColors();
         ui->glMain->reBuffer(var.tarObj);
    }
    this->setFocus();
}

void MainWindow::on_selection_stateChanged(int arg1)
{
    ui->glMain->setSelectMode(arg1>0);
    if(arg1){
        support.getModel(var.tarObj)->applyModelMatrix();
    }else{

    }
    this->setFocus();
}

void MainWindow::on_btnNormalize_clicked()
{
    if(ui->glMain->getTarnum()<1)return;
    support.getModel(var.tarObj)->normalize(var.normalizeVal);
    support.getModel(var.tarObj)->applyModelMatrix();
    ui->glMain->reBuffer(0);
    this->setFocus();
}

void MainWindow::on_btnCurv_clicked()
{

    QMessageBox qmb;
    qmb.setStandardButtons(NULL);
    qmb.setText(tr("                         "));
    qmb.setWindowTitle(tr("Calculating..."));
    qmb.show();
    //getModel(var.tarObj)->regenNormals();

    support.getModel(var.tarObj)->calNeighbor();

    //clock_t t1 = clock();
    support.getModel(var.tarObj)->calCurvures(1);
    //clock_t t2 = clock();
    //printf("%lf", (t2-t1)/(double)(CLOCKS_PER_SEC));
    //std::cout << std::endl;

    support.getModel(var.tarObj)->paintCurvures();
    qmb.close();
    ui->glMain->reBuffer(var.tarObj);
    this->setFocus();
}

void MainWindow::on_btnDetour_clicked()//tune framework
{
    int flag;
    if(var.skelMgr.loaded){
            QVector3D c = var.cuttingpoint[var.cutcnt];
            QVector3D n = var.cuttingpnorm[var.cutcnt];
            flag = support.getModel(var.tarObj)->calDetourByPlane(c,n);
    }else if(support.getModel(var.tarObj)->selecPoints.size()==3){
        flag = support.getModel(var.tarObj)->calDetourByPlane();
    }else{
        return;
    }
    if(flag==1){
        support.getModel(var.tarObj)->setColors(0.5f,0.5f,0.5f);
        support.getModel(var.tarObj)->paintDetour();
    }else{
        ui->glMain->deleteTar_ass(var.assistPlaneIdx);
    }
    support.getModel(var.tarObj)->connectorFaceIdxs.clear();
    ui->glMain->reBuffer(var.tarObj);
}

void MainWindow::on_cmbbSelection_currentIndexChanged(int index)
{
    ui->glMain->curSelecTar=index;
}

void MainWindow::on_btnReset_clicked()
{

    for(int i=ui->glMain->getTarnum()-1;i>var.tarObj;i--)ui->glMain->deleteTar(i);
    support.getModel(var.tarObj)->setColors(0.5f,0.5f,0.5f);
    support.getModel(var.tarObj)->refresh();
    support.getModel(var.tarObj)->ResetModel();
    ui->glMain->reBuffer(var.tarObj);
}

void MainWindow::on_showPlate_stateChanged(int arg1)
{
    if(support.getModel(var.tarObj)->selecPoints.size()==3){
        int tarIdx_ass = 0;
        if(arg1!=0){
            ui->glMain->assistMode = 1;
            ui->glMain->generateAssitDisc(var.tarObj);
        }else{
            ui->glMain->setVis_ass(tarIdx_ass, 0);
            ui->glMain->assistMode = 0;
        }
    }else if(var.skelMgr.loaded){
        int tarIdx_ass = 0;
        if(arg1!=0){
            ui->glMain->assistMode = 1;
            QVector3D c=var.cuttingpoint[var.cutcnt];
            QVector3D n=var.cuttingpnorm[var.cutcnt];
            float r = 30.0f;
            ui->glMain->generateAssitDisc(c,n,r);
        }else{
            ui->glMain->setVis_ass(tarIdx_ass, 0);
            ui->glMain->assistMode = 0;
        }
    }

    ui->glMain->update();
}

void MainWindow::on_btnCut_clicked()
{
    if(support.getModel(var.tarObj)->selecIdxs.size()==0&&!var.skelMgr.loaded)return;
    if(support.getModel(var.tarObj)->detourIdxs.size()==0)return;
    support.deleteSkelSup();
    char str[20];
    int tarnum = ui->glMain->getTarnum();
    ui->glMain->copyObj(var.tarObj);

    support.getModel(var.tarObj)->cutByDetour(1);
    support.cgaltool.fillHole(support.getModel(var.tarObj)->vertices_ori, support.getModel(var.tarObj)->indices);
    //ui->glMain->reload("./temp.obj",var.tarObj);
        support.iglMachine.reset();support.iglMachine.readFile("temp","temp.off");
        support.iglMachine.get("temp",support.getModel(var.tarObj)->vertices_ori, support.getModel(var.tarObj)->indices);
        support.getModel(var.tarObj)->applyModelMatrix_force();
    support.getModel(var.tarObj)->refresh();
    ui->glMain->reBuffer(var.tarObj);

    support.getModel(tarnum)->cutByDetour(-1);
    support.cgaltool.fillHole(support.getModel(tarnum)->vertices_ori, support.getModel(tarnum)->indices);
    //ui->glMain->reload("./temp.obj",tarnum);
        support.iglMachine.reset();support.iglMachine.readFile("temp","temp.off");
        support.iglMachine.get("temp",support.getModel(tarnum)->vertices_ori, support.getModel(tarnum)->indices);
        support.getModel(tarnum)->applyModelMatrix_force();
    support.getModel(tarnum)->refresh();
    ui->glMain->reBuffer(tarnum);

    sprintf(str,"Object %d",tarnum+1);
    ui->cmbbObject->addItem(QString(str));
    ui->glMain->deleteTar_ass(var.assistPlaneIdx);
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        //ui->showSkel->setCheckable(true);//locked, so no need; and path.length>0 must be skel mode
        ui->showSkel->setChecked(false);
    }
}

void MainWindow::on_btnOutput_clicked()
{
    support.iglMachine.reset();
    support.iglMachine.put("mainobj", support.getModel(var.tarObj)->vertices, support.getModel(var.tarObj)->indices);
    char str[20];
    sprintf(str,"test.obj");
    support.iglMachine.writeFile("mainobj",str);
}

void MainWindow::on_cmbbObject_currentIndexChanged(int index)
{
    for(int i=0;i<ui->glMain->getTarnum();i++){
        if(i==index)ui->glMain->setVis(i,1);
        else ui->glMain->setVis(i,0);
    }
    var.tarObj = index;
    ui->glMain->tarObj = var.tarObj;
    ui->glMain->update();
}

void MainWindow::on_btnPushConnector_clicked()
{
    if(!support.getModel(var.tarObj)->connectorFaceReady){
        support.drawCircleOnPlane();
    }
    support.getModel(var.tarObj)->pushConnect(5.0f);
    ui->glMain->reBuffer(var.tarObj);
}

void MainWindow::on_btnPullConnector_clicked()
{
    if(!support.getModel(var.tarObj)->connectorFaceReady){
        support.drawCircleOnPlane();
    }
    support.getModel(var.tarObj)->pullConnect(5.0f);
    ui->glMain->reBuffer(var.tarObj);
}

void MainWindow::on_btnPutConnector_clicked()
{
    if(!support.getModel(var.tarObj)->connectorReady){
        return;
    }
    if(var.connectPuted == true){
        ui->glMain->deleteTar(var.connectTarNum);
    }
    float s = support.getModel(var.tarObj)->GetScaleXYZ().x();
    float r = support.getModel(var.tarObj)->connectorRadii_ori * s * 1.6;
    QVector3D c = support.getModel(var.tarObj)->connectorCenter_ori*s;
    QVector3D n = support.getModel(var.tarObj)->connectorNormal_ori;
    if(var.connectType == 0){
        support.putStdModel("cylinder10X10", QVector3D(1.0f,0.5f,0.5f),QVector3D(r/100,r/100,0.1f),c,n);

    }else if(var.connectType == 1){
        r/=1.41421;
        support.putStdModel("cube10X10", QVector3D(1.0f,0.5f,0.5f),QVector3D(r/100,r/100,0.1f),c,n);
    }else{
        ui->glMain->deleteTar(var.connectTarNum);
    }
    var.connectType= (var.connectType+1)%(var.totalconnectType+1);
    var.connectPuted = true;
}

void MainWindow::on_btnSwell_clicked()
{
    if(!var.connectPuted)return;
    support.applyCSG('+',var.tarObj,var.connectTarNum);
    var.connectPuted=false;
    ui->glMain->deleteTar(var.connectTarNum);
    support.getModel(var.tarObj)->connectorReady=false;
}

void MainWindow::on_btnDig_clicked()
{
    if(!var.connectPuted)return;
    support.applyCSG('-',var.tarObj,var.connectTarNum);
    var.connectPuted=false;
    ui->glMain->deleteTar(var.connectTarNum);
    support.getModel(var.tarObj)->connectorReady=false;
}

void MainWindow::on_btnDrawCir_clicked()
{
    //support.drawCircleOnPlane();
    support.fill(var.tarObj);
    ui->glMain->reBuffer(var.tarObj);
}

void MainWindow::on_btnLoadSkel_clicked()
{
    if(ui->glMain->getTarnum()<1)return;
    ui->glMain->deleteTar_ass(var.assistPlaneIdx);
    support.getModel(var.tarObj)->refresh();
    ui->glMain->reBuffer(var.tarObj);
    support.deleteSkelSup();
    if(var.quickmode){
        var.skelMgr.path="./skeltest.txt";
        support.loadSkelSup();
    }else{
        var.skelMgr.path = QFileDialog::getOpenFileName(this, tr("Load obj"), "..", tr("Text Files(*.txt)")).toStdString();
        if(var.skelMgr.path.length() == 0) {
                QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        } else {
            support.loadSkelSup();
        }
    }
    var.cutcnt=0;
    this->setFocus();
}
void MainWindow::on_btnCutSkel_clicked()
{
    if(!var.skelMgr.loaded)return;
    var.cutcnt = (var.cutcnt+1)%8;
    QVector3D c=var.cuttingpoint[var.cutcnt];
    QVector3D n=var.cuttingpnorm[var.cutcnt];
    float r = 30.0f;
    ui->glMain->generateAssitDisc(c,n,r);
    ui->showPlate->setChecked(true);
}

void MainWindow::on_showSkel_stateChanged(int arg1)
{
    if(var.skelMgr.loaded){
        if(arg1!=0){
            for(int i=ui->glMain->getTarnum()-10;i<ui->glMain->getTarnum();i++){
                ui->glMain->setVis(i, 1);
            }
        }else{
            for(int i=ui->glMain->getTarnum()-10;i<ui->glMain->getTarnum();i++){
                ui->glMain->setVis(i, 0);
            }
        }
    }
    if(ui->glMain->getTarnum()>0
    && support.getModel(var.tarObj)->selecPoints.size()>0){
        if(arg1!=0){
            support.getModel(var.tarObj)->refresh();
        }else{
            support.deleteSkelSup();
            var.skelMgr.Reset();
        }
    }
    ui->glMain->update();
}
void MainWindow::on_edgeNum_valueChanged(int arg1)
{
    support.getModel(var.tarObj)->clearConnector();
    ui->glMain->reBuffer(var.tarObj);
}
