#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = 0;
    setWindowFlags(flags);
    //setFixedSize(800,520);
    setFixedSize(800,520);
    /******************/
    flags |= Qt::WindowMinimizeButtonHint;
    support = MainWindow_support(ui, &var);
    if(ctrlable){
        maintainthread.setupVar(&var);
        maintainthread.setupUi(ui);
        maintainthread.start();
    }
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
        ui->glMain->load(path.data());

    }else{
        std::string path = QFileDialog::getOpenFileName(this, tr("Load obj"), "..", tr("Object Files(*.obj)")).toStdString();
        if(path.length() == 0) {
                QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        } else {
                //QMessageBox::information(NULL, tr("Path"),tr(path.data()));
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
    for(int i=ui->cmbbObject->count()-1;i>=0;i--){
        ui->cmbbObject->removeItem(i);
    }
    ui->cmbbObject->addItem(QString(str));
    this->setFocus();
}


void MainWindow::usedetails3(){
    support.deleteSkelSup();
    /************/
    float angleDif = std::abs(ui->sldRotate->value()-50.0f)/50*TileLimit;
    float twistAngle = tan((45.0f-angleDif)/180*M_PI)*180/M_PI;
    twistAngle = curTwistAngle;
    /************/
    for(int i=0;i<tar2s.size();i++){
        float r = rs[i];
        QVector3D c = cs[i];
        QVector3D n = ns[i];
        int tar1 = tar1s[i];
        int tar2 = tar2s[i];
        /***************************/
        bool cav = false;
        if(cav){
            ui->glMain->genSpiral(c,n,r, 1, HDiv, twistAngle, 0.7f, 0.9f);
            int cutnum = ui->glMain->getTarnum()-1;
            ui->glMain->genSpiralCav(c,n,r, 1);
            int cutnum2 = ui->glMain->getTarnum()-1;
            support.applyCSG('-', cutnum, cutnum2);
            support.applyCSG('+', tar1, cutnum);
            ui->glMain->deleteTar(cutnum);
            ui->glMain->genSpiral_withhead(c,n,r, 1, HDiv, twistAngle, 0.8f, 1.0f, angleDif);
            cutnum = ui->glMain->getTarnum()-1;
            support.applyCSG('-', tar2, cutnum);
            ui->glMain->deleteTar(cutnum);
            ui->glMain->deleteTar(cutnum2);
        }else{
            ui->glMain->genSpiral(c,n,r, 1, HDiv, twistAngle, 0.7f, 0.9f);
            int cutnum = ui->glMain->getTarnum()-1;
            support.applyCSG('+', tar1, cutnum);
            ui->glMain->deleteTar(cutnum);
            ui->glMain->genSpiral_withhead(c,n,r, 1, HDiv, twistAngle, 0.8f, 1.0f, angleDif);
            cutnum = ui->glMain->getTarnum()-1;
            support.applyCSG('-', tar2, cutnum);
            ui->glMain->deleteTar(cutnum);
        }
    }
    /************/
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        ui->showSkel->setChecked(false);
    }
}
void MainWindow::usedetails4(){
    support.deleteSkelSup();
    /************/
    float angleDif = std::abs(ui->sldRotate->value()-50.0f)/50*TileLimit;
    float twistAngle = tan((45.0f-angleDif)/180*M_PI)*180/M_PI;
    twistAngle = curTwistAngle;
    /************/
    for(int i=0;i<tar2s.size();i++){
        float r = rs[i];
        QVector3D c = cs[i];
        QVector3D n = n2s[i];
        int tar1 = tar1s[i];
        int tar2 = tar2s[i];
        //std::cout << "break" << std::endl;
        /***************************/
        ui->glMain->genTrack(c,n,r, true);
        int cutnum = ui->glMain->getTarnum()-1;
        support.applyCSG('*', cutnum, tar2);

        ui->glMain->genTrack(c,n,r, false);
        int cutnum2 = ui->glMain->getTarnum()-1;
        support.applyCSG('+', cutnum2, tar1);

        support.applyCSG('+', tar1, cutnum);
        support.applyCSG('-', tar2, cutnum2);

        ui->glMain->deleteTar(cutnum);
        ui->glMain->deleteTar(cutnum2);
    }
    /************/
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        ui->showSkel->setChecked(false);
    }
}

void MainWindow::usedetails5(){
    support.deleteSkelSup();
    /************/
    float angleDif = std::abs(ui->sldRotate->value()-50.0f)/50*TileLimit;
    float twistAngle = tan((45.0f-angleDif)/180*M_PI)*180/M_PI;
    twistAngle = curTwistAngle;
    /************/
    for(int i=0;i<tar2s.size();i++){
        float r = rs[i];
        QVector3D c = cs[i];
        QVector3D n = n2s[i];
        int tar1 = tar1s[i];
        int tar2 = tar2s[i];
        //std::cout << "break" << std::endl;
        /***************************/
        if(connTypes[i]==1){
            ui->glMain->genTrack(c,n,r, true);
            int cutnum = ui->glMain->getTarnum()-1;
            support.applyCSG('*', cutnum, tar2);

            ui->glMain->genTrack(c,n,r, false);
            int cutnum2 = ui->glMain->getTarnum()-1;
            support.applyCSG('+', cutnum2, tar1);

            support.applyCSG('+', tar1, cutnum);
            support.applyCSG('-', tar2, cutnum2);

            ui->glMain->deleteTar(cutnum);
            ui->glMain->deleteTar(cutnum2);
        }
        /***************************/
        if(connTypes[i]==0){
            ui->glMain->genSpiral(c,n,r, 1, HDiv, twistAngle, 0.8f, 1.0f);
            int cutnum = ui->glMain->getTarnum()-1;
            support.applyCSG('+', tar1, cutnum);
            ui->glMain->deleteTar(cutnum);
            ui->glMain->genSpiral_withhead(c,n,r, 1, HDiv, twistAngle, 0.8f, 1.0f, angleDif);
            cutnum = ui->glMain->getTarnum()-1;
            support.applyCSG('-', tar2, cutnum);
            ui->glMain->deleteTar(cutnum);
        }
    }
    /************/
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        ui->showSkel->setChecked(false);
    }
}

void MainWindow::usedetails2(){
    support.deleteSkelSup();
    /************/
    for(int i=0;i<tar2s.size();i++){
        float s = ss[i];
        float r = rs[i];
        QVector3D c = cs[i];
        QVector3D n = ns[i];
        int tar1 = tar1s[i];
        int tar2 = tar2s[i];

        ui->glMain->genSpiral_withhead(c,n,r/2, 1, 100, 90,0.8f, 1.0f, 0);
        int cutnum = ui->glMain->getTarnum()-1;
        support.applyCSG('-', tar1, cutnum);
        ui->glMain->deleteTar(cutnum);
        ui->glMain->genSpiral(c,n,r/2, 1, 100, 90,0.8f, 1.0f);
        cutnum = ui->glMain->getTarnum()-1;

        support.applyCSG('+', tar2, cutnum);
        ui->glMain->deleteTar(cutnum);
    }
    /************/
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        ui->showSkel->setChecked(false);
    }
}

void MainWindow::usedetails(){
    support.deleteSkelSup();
    /************/
    for(int i=0;i<tar2s.size();i++){
        float s = ss[i];
        float r = rs[i];
        QVector3D c = cs[i];
        QVector3D n = ns[i];
        int tar1 = tar1s[i];
        int tar2 = tar2s[i];
        if(i==3||i==0){
            ui->glMain->genSpiral_withhead(c,n,r/2, 1, 100, 90,0.8f, 1.0f, 0);
            int cutnum = ui->glMain->getTarnum()-1;
            support.applyCSG('-', tar1, cutnum);
            ui->glMain->deleteTar(cutnum);
            ui->glMain->genSpiral(c,n,r/2, 1, 100, 90,0.8f, 1.0f);
            cutnum = ui->glMain->getTarnum()-1;

            support.applyCSG('+', tar2, cutnum);
            ui->glMain->deleteTar(cutnum);
        }else{
            ui->glMain->copyObj(tar1);
            int addnum = ui->glMain->getTarnum()-1;
            support.putStdModel("spcb", QVector3D(1.0f,0.5f,0.5f),QVector3D(r/100,r/100,r/100),c,n);
            int cutnum = ui->glMain->getTarnum()-1;
            support.applyCSG('-', tar1, cutnum);
            ui->glMain->deleteTar(cutnum);
            support.putStdModel("spca", QVector3D(1.0f,0.5f,0.5f),QVector3D(r/100,r/100,r/100),c,n);
            cutnum = ui->glMain->getTarnum()-1;
            //support.applyCSG('*', addnum, cutnum);
            //support.applyCSG('+', tar2, addnum);
            support.applyCSG('+', tar2, cutnum);
            ui->glMain->deleteTar(cutnum);
            ui->glMain->deleteTar(addnum);
        }
    }
    /************/
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        ui->showSkel->setChecked(false);
    }
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


void MainWindow::on_btnReset_clicked()
{

    for(int i=ui->glMain->getTarnum()-1;i>var.tarObj;i--)ui->glMain->deleteTar(i);
    support.getModel(var.tarObj)->setColors(0.5f,0.5f,0.5f);
    support.getModel(var.tarObj)->refresh();
    support.getModel(var.tarObj)->ResetModel();
    ui->glMain->reBuffer(var.tarObj);
}



void MainWindow::on_btnCut_clicked()
{
    var.lock();
    if(support.getModel(var.tarObj)->selecIdxs.size()==0&&!var.skelMgr.loaded)return;
    if(support.getModel(var.tarObj)->detourIdxs.size()==0)return;
    support.deleteSkelSup();
    char str[20];
    int tarnum = ui->glMain->getTarnum();

    ui->glMain->copyObj(var.tarObj);
    support.getModel(var.tarObj)->cutByDetour(1);
    std::string mstr = support.cgaltool.fillHoleAndGetStr(support.getModel(var.tarObj)->vertices_ori, support.getModel(var.tarObj)->indices);
    support.cgaltool.readFromOFFStream(support.getModel(var.tarObj)->vertices_ori, support.getModel(var.tarObj)->indices,mstr);
    mstr.clear();
        //ui->glMain->reload("./temp.obj",var.tarObj);
            //support.iglMachine.reset();support.iglMachine.readFile("temp","temp.off");
            //support.iglMachine.get("temp",support.getModel(var.tarObj)->vertices_ori, support.getModel(var.tarObj)->indices);
    support.getModel(var.tarObj)->applyModelMatrix_force();
    support.getModel(var.tarObj)->refresh();
    ui->glMain->reBuffer(var.tarObj);

    support.getModel(tarnum)->cutByDetour(-1);
    mstr = support.cgaltool.fillHoleAndGetStr(support.getModel(tarnum)->vertices_ori, support.getModel(tarnum)->indices);
    support.cgaltool.readFromOFFStream(support.getModel(tarnum)->vertices_ori, support.getModel(tarnum)->indices, mstr);
    mstr.clear();
        //ui->glMain->reload("./temp.obj",tarnum);
            //support.iglMachine.reset();support.iglMachine.readFile("temp","temp.off");
            //support.iglMachine.get("temp",support.getModel(tarnum)->vertices_ori, support.getModel(tarnum)->indices);
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
    var.unlock();
    std::cout << "cut on " << var.tarObj << std::endl;
}

void MainWindow::on_btnOutput_clicked()
{
    support.iglMachine.reset();
    support.iglMachine.put("mainobj", support.getModel(var.tarObj)->vertices, support.getModel(var.tarObj)->indices);
    char str[20];
    sprintf(str,"test.obj");
    support.iglMachine.writeFile("mainobj",str);
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
    QVector3D c;
    QVector3D n;
    if(!var.skelMgr.loaded){
        /*
        var.cutcnt = 0;
        var.cuttingpoint.clear();var.cuttingpnorm.clear();
        var.cuttingpoint.push_back(QVector3D(0,0,0));
        var.cuttingpnorm.push_back(QVector3D(0,0,1));
        var.cuttingpnorm2.push_back(QVector3D(0,0,-1));
        c=var.cuttingpoint[var.cutcnt];
        n=var.cuttingpnorm[var.cutcnt];
        */
    }else{
        var.cutcnt = (var.cutcnt+1)%var.cuttingpoint.size();
        c=var.cuttingpoint[var.cutcnt];
        n=var.cuttingpnorm[var.cutcnt];
    }
//    printf("%f/%f/%f\n",c.x(),c.y(),c.z());
//    printf("%f/%f/%f\n",n.x(),n.y(),n.z());
//    std::cout << std::endl;

    float r = 15.0f;
    ui->glMain->generateAssitDisc(c,n,r);
    ui->glMain->setVis_ass(1,0);
    ui->glMain->setVis_ass(2,0);
    //ui->glMain->generateAssitDisc(c+n.normalized()*r*2,n,r,1);
    //ui->glMain->generateAssitPillar(c, n, r/2, 100);
    ui->showPlate->setChecked(true);
}

void MainWindow::on_btnLoad_Manu_clicked()
{
    std::string path = QFileDialog::getOpenFileName(this, tr("Load obj"), "..", tr("Object Files(*.obj)")).toStdString();
    if(path.length() == 0) {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    } else {
            //QMessageBox::information(NULL, tr("Path"),tr(path.data()));
            QMessageBox qmb;
            qmb.setStandardButtons(NULL);
            qmb.setText(tr("                     "));
            qmb.setWindowTitle(tr("Loading......."));
            qmb.show();
            ui->glMain->load(path.data());
            qmb.close();
    }
    char str[10];
    sprintf(str,"Object %d",ui->glMain->getTarnum());
    ui->cmbbObject->addItem(QString(str));
    ui->cmbbObject->setCurrentIndex((var.tarObj));
    this->setFocus();
}

void MainWindow::on_btnLoadHead_clicked()
{

    if(!var.skelMgr.loaded)return;
    support.deleteSkelSup();
    QVector3D c=var.cuttingpoint[0];
    QVector3D n=var.cuttingpnorm[0];
    /***************/

    std::string path="./head.obj";
    ui->glMain->load(path.data());


    //ui->glMain->viewMgr->Reset();

    var.tarObj = ui->glMain->getTarnum()-1;
    support.getModel(var.tarObj)->scaleDepend_ori(QVector3D(0,0,0),50, 50, 50);
    support.getModel(var.tarObj)->translate_pure(c+QVector3D(0,30,0));
    //support.getModel(var.tarObj)->rotateTo_u(n);
    support.getModel(var.tarObj)->applyModelMatrix_force();
    ui->glMain->setVis(var.tarObj,1);
    ui->glMain->reBuffer(var.tarObj);

    char str[10];
    sprintf(str,"Object %d",ui->glMain->getTarnum());
    ui->cmbbObject->addItem(QString(str));
    ui->cmbbObject->setCurrentIndex((var.tarObj));
    /******************************************/
    QVector3D p = var.skelMgr.partPosition("neck");
    support.putStdModel("cylinder10X10",QVector3D(0.5f,0.5f,0.5f),QVector3D(0.1f,0.1f,0.2f),p,QVector3D(0,1,0));
    support.applyCSG('+', 0, 10);
    support.applyCSG('-', 9, 0);
    ui->glMain->deleteTar(10);
    /***************/
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        ui->showSkel->setChecked(false);
    }
}

void MainWindow::on_btnArea_clicked()
{
    char str[20];
    sprintf(str,"Area : %d", (int)support.getModel(var.tarObj)->calCutArea());
    ui->btnArea->setText(QString(str));
}

void MainWindow::on_btnArea_2_clicked()
{
    char str[20];
    float radii;
    float angleCal;
    float area = support.getModel(var.tarObj)->calCutArea(var.cuttingpoint[var.cutcnt],var.cuttingpnorm[var.cutcnt], radii, true, angleCal);
    QVector3D c = var.cuttingpoint[var.cutcnt];
    QVector3D n = var.cuttingpnorm[var.cutcnt];
    ui->glMain->generateAssitDisc(c, n,radii*1.5f);
    ui->glMain->generateAssitDisc(c+n.normalized()*radii,n,radii*1.5f,1);
    ui->glMain->generateAssitPillar(c, n, radii * 0.9f, 50);
    ui->glMain->update();
    sprintf(str,"Area : %d", (int)area);
    ui->btnArea_2->setText(QString(str));
    radiis[var.cutcnt] = radii;
}

void MainWindow::on_btnKinect_clicked()
{
#ifdef __APPLE__
    std::cout << "mac" << std::endl;
    system("./Simple");
#else
    std::cout << "win" << std::endl;
    system("Simple.exe");
#endif
}

void MainWindow::on_btnLoadAll_clicked()
{
    ui->btnLoad->click();
    ui->btnLoadSkel->click();
    ui->showSkel->setChecked(false);
    loaded = true;
}

void MainWindow::on_btnCutAll_clicked()
{
    QMessageBox qmb;
    qmb.setStandardButtons(NULL);
    qmb.setText(tr("                     "));
    qmb.setWindowTitle(tr("Loading......."));
    qmb.show();

    //ui->btnTest3->click();
    /*************************************/

    ui->btnCutSkel->click();
    ui->btnOptAll->click();
    ui->btnApplyOpt->click();

    ui->btnCutSkel->click();
    ui->btnOptAll->click();
    ui->btnApplyOpt->click();

    ui->btnCutSkel->click();
    ui->btnOptAll->click();
    ui->btnApplyOpt->click();

    connType = 1;
    ui->cmbbObject->setCurrentIndex(1);
    ui->btnCutSkel->click();
    ui->btnOptAll->click();
    ui->btnApplyOpt->click();

    ui->cmbbObject->setCurrentIndex(2);
    ui->btnCutSkel->click();
    ui->btnOptAll->click();
    ui->btnApplyOpt->click();

    ui->cmbbObject->setCurrentIndex(3);
    ui->btnCutSkel->click();
    ui->btnOptAll->click();
    ui->btnApplyOpt->click();

    ui->btnCutSkel->click();
    ui->btnOptAll->click();
    ui->btnApplyOpt->click();
    connType = 0;
    /*************************************/
    ui->btnApplyCut->click();
    ui->btnAddPlate->click();
    ui->btnLoadHead->click();
    support.deleteSkelSup();
    ui->glMain->setColor_mySet();
    ui->glMain->setAllVis(1);

    /*************************************/
    /*
    ui->btnLoadHead->click();
    support.deleteSkelSup();
    ui->glMain->setColor_mySet();
    ui->glMain->setAllVis(1);
    */
    qmb.close();
}

void MainWindow::on_btnOutputAll_clicked()
{
    ui->btnPrintRot->click();
    system("mkdir output");
    for(int i=0;i<ui->cmbbObject->count();i++){
        support.iglMachine.reset();
        support.iglMachine.put("mainobj", support.getModel(i)->vertices, support.getModel(i)->indices);
        char str[20];
        sprintf(str,"%d.obj",i);
        support.iglMachine.writeFile("mainobj",str);
#ifdef __APPLE__
        sprintf(str,"mv %d.obj output/%d.obj",i,i);
#else
        sprintf(str,"move %d.obj output/%d.obj",i,i);
#endif
        system(str);
    }
    support.stateInit();
    for(int i=ui->cmbbObject->count()-1;i>=0;i--){
        ui->cmbbObject->removeItem(i);
    }
}

void MainWindow::on_btnAddPlate_clicked()
{
    support.deleteSkelSup();
    /***************/
    QVector3D p1, p2, p;
    p1 = var.skelMgr.partPosition("feet_left");
    p2 = var.skelMgr.partPosition("feet_right");
    p = (p1.y()>p2.y())?p2:p1;
    int tar = (p1.y()>p2.y())?7: 6;
    //support.putStdModel("cube10X10",QVector3D(0.5f,0.5f,0.5f),QVector3D(0.5f,0.1f,0.5f),QVector3D(0,std::fmin(p1, p2),0),QVector3D(0,1,0));
    support.putStdModel("cube10X10",QVector3D(0.5f,0.5f,0.5f),QVector3D(0.5f,0.5f,0.1f),p-QVector3D(0,14.9f,0),QVector3D(0,1,0));
    support.putStdModel("cylinder10X10",QVector3D(0.5f,0.5f,0.5f),QVector3D(0.1f,0.1f,0.2f),p-QVector3D(0,10.0f,0),QVector3D(0,1,0));
    char str[10];
    sprintf(str,"Object %d",8+1);
    ui->cmbbObject->addItem(QString(str));
    //support.applyCSG('-',var.tarObj+1,var.tarObj);
    support.applyCSG('-',8,  9);
    support.applyCSG('+',tar,9);
    ui->glMain->deleteTar(9);
    //ui->glMain->deleteTar(8);
    /***************/
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        ui->showSkel->setChecked(false);
    }
}

void MainWindow::onConnChange(){
    if(!var.skelMgr.loaded){
        float trans = (float)ui->sldTranslate->value()/100*360 - 180;
        float rot = (float)ui->sldRotate->value()/100*360 - 180;
        float tile = (float)ui->sldTile->value()/50.0f;
        QMatrix4x4 mat;
        mat.rotate(rot, QVector3D(0,0,1));
        mat.rotate(trans, QVector3D(1,0,0));
        QVector3D c = mat * QVector3D(0,0,0);
        QVector3D n = mat * QVector3D(0,1,0);
        float r = 50, l = 50.0f*tile;
        ui->glMain->generateAssitDisc(c, n, r);
        ui->glMain->generateAssitDisc(c, n, r, 1);
        ui->glMain->generateAssitPillar(c, n, r, l);
        ui->glMain->setVis_ass(0, 1);
        ui->glMain->setVis_ass(1, 1);
        ui->glMain->setVis_ass(2, 1);


        char str2[20];
        sprintf(str2,"%d", support.getModel(var.tarObj)->checkColumnBound(c, n, r, l)?1:0);
        ui->btnArea_2->setText(QString(str2));
        return;
    }
    /****************************************/
    //std::cout << ui->sldTranslate->value() << " ";
    //std::cout << ui->sldRotate->value() << " ";
    //std::cout << ui->sldTile->value() << " ";
    //std::cout << ui->sldR->value() << " " << std::endl;
    float rotRange = 90;
    float trans = ((float)ui->sldTranslate->value()-50)/5;
    float rot = (float)ui->sldRotate->value()/100*rotRange*2 - rotRange;
    float tile = (float)ui->sldTile->value()/100*TileLimit*2 - TileLimit;
    float rate = 1-(float)ui->sldR->value()/200;
    /****************************************/
    curPlateRotVal = rot;
    curPlateTileVal = tile;
    //radiis[var.cutcnt] = support.getModel(var.tarObj)->calCutArea(var.cuttingpoint[var.cutcnt],var.cuttingpnorm[var.cutcnt]);
    float radii;// = radiis[var.cutcnt];
    QVector3D c = var.cuttingpoint[var.cutcnt];
    QVector3D n = var.cuttingpnorm[var.cutcnt];
    QVector3D n2 = var.cuttingpnorm2[var.cutcnt];
    Arg arg = Arg(c, n, n2, radii, trans, rot, tile, rate);
    float angleCal;
    QVector3D cfix = arg.cfix;
    QVector3D nplate = arg.nplate;
    QVector3D npillar = arg.npillar;
    c = arg.c;
    float area = support.getModel(var.tarObj)->calCutArea(c,nplate, radii, autofix, angleCal);
    arg.setRadii(radii);
    float tuneRadii = arg.tuneRadii;
    cfix = arg.cfix;
    if(drawass)ui->glMain->setColor(var.tarObj, 0.5f, 0.5f, 0.5f);
    curPilarnValid = support.getModel(var.tarObj)->checkColumnBound(c - cfix, npillar, tuneRadii, tuneRadii);
    if(drawass){
        ui->glMain->generateAssitDisc(c, nplate, radii*2.0f);
        ui->glMain->generateAssitDisc(c - cfix+npillar.normalized()*radii,npillar,radii*2.0f,1);
        ui->glMain->generateAssitPillar(c - cfix, npillar, tuneRadii, 50);

        if(curPilarnValid)ui->glMain->setColor_ass(2, 1.0f, 0.5f, 0.5f);
        else ui->glMain->setColor_ass(2, 0.5f, 1.0f, 0.5f);

        char str[20];
        sprintf(str,"Area : %d ", (int)area);
        ui->btnArea_2->setText(QString(str));
        ui->glMain->reBuffer(var.tarObj);
    }
    curArea = area;
    curC = c;
    curPlateN = nplate;
    curPilarC = c - cfix;
    curPilarN = npillar;
    curPlateR = radii;
    curPilarR = tuneRadii;
    curAreaRate = curPilarR*curPilarR*M_PI/curArea;
    curAngleCal = angleCal;
    //std::cout << "angleCal : " << angleCal << std::endl;

    /*
    std::cout << curArea <<std::endl;
    std::cout << curC.z()<<std::endl;
    std::cout << curPlateN.z() <<std::endl;
    std::cout << curPilarC.z() <<std::endl;
    std::cout << curPilarN.z() <<std::endl;
    std::cout << curPilarR <<std::endl;
    */
}
void MainWindow::on_btnArgrst_clicked()
{
    ui->sldTile->setValue(50);
    ui->sldRotate->setValue(50);
    ui->sldTranslate->setValue(50);
    ui->sldR->setValue(0);
}

void MainWindow::on_btnOpt_clicked()
{
    autofix = true;
    drawass = false;
    float minArea = FLT_MAX;
    float minAreaRot;
    bool ignore = false;
    for(int i=50;i<=100&&!ignore;i++){
        if(ignore)break;
        ui->sldRotate->setValue(i);
        onConnChange();
        if(curArea<minArea){
            minArea=curArea;
            minAreaRot = i;
            if(fastOptMode && std::abs(curPlateRotVal)>acceptableRange){
                ignore = true;
                break;
            }
        }
    }
    for(int i=50;i>=0&&!ignore;i--){
        ui->sldRotate->setValue(i);
        onConnChange();
        if(curArea<minArea){
            minArea=curArea;
            minAreaRot = i;
            if(fastOptMode && std::abs(curPlateRotVal)>acceptableRange){
                ignore = true;
                break;
            }
        }
    }
    autofix = false;
    drawass = true;
    ui->sldRotate->setValue(minAreaRot);
    onConnChange();
}

void MainWindow::on_btnOpt2_clicked()
{
    autofix = true;
    drawass = false;
    float maxDot = FLT_MIN;
    float maxDotTile;
    /*
    for(int i=0;i<=100;i++){
        ui->sldTile->setValue(i);
        onConnChange();
        float dot = QVector3D::dotProduct(var.cuttingpnorm[var.cutcnt].normalized(), curPilarN.normalized());
        if(maxDot<dot){
            maxDot = dot;
            maxDotTile = i;
        }
    }
    */
    //maxDotTile = 100-ui->sldRotate->value();
    float minangle = std::min(std::abs(curPlateRotVal),TileLimit);
    float sldvalue = minangle / TileLimit * 50;
    if(curPlateRotVal>=0){
        maxDotTile = 50-(int)sldvalue;
    }else{
        maxDotTile = 50+(int)sldvalue;
    }


    autofix = false;
    drawass = true;
    ui->sldTile->setValue(maxDotTile);
    onConnChange();
}

void MainWindow::on_btnOpt3_clicked()
{
    ui->sldR->setValue(100);
    onConnChange();
    if(curPilarnValid)return;
    ui->sldR->setValue(0);
    onConnChange();
    if(!curPilarnValid)return;

    int lower = 0, upper = 100;
    while(lower<upper){
        int mid = (lower+upper)/2;
        ui->sldR->setValue(mid);
        onConnChange();
        if(curPilarnValid){
            lower = mid+1;
        }
        else{
            upper = mid;
        }
    }
    ui->sldR->setValue(lower);
    onConnChange();
    /*
    for(int i=0;i<=100;i++){
        ui->sldR->setValue(i);
        onConnChange();
        if(!curPilarnValid)return;
    }
    */
}

void MainWindow::on_btnOptAll_clicked()
{
    if(connType==0){
        for(int i=50;i<=100;i++){
            ui->btnArgrst->clicked();
            ui->sldTranslate->setValue(i);
            onConnChange();
            ui->btnOpt->click();
            if(std::abs(curPlateRotVal)>acceptableRange)continue;
            ui->btnOpt2->click();
            ui->btnOpt3->click();
            /**********************/
            /*
            float limit = 25.0f;
            if(QVector3D::dotProduct(curPilarN.normalized(),curPlateN.normalized())>cos(limit*M_PI/180))return;
            */
            //if(std::abs(curPlateRotVal)>acceptableRange)continue;
            if(!curPilarnValid)break;
        }
        float maxAngleCal = FLT_MIN;
        int maxAngleCalVal = -1;
        int tinit = ui->sldTranslate->value();
        for(int i=tinit;i<tinit+10;i++){
            ui->sldTranslate->setValue(i);
            onConnChange();
            if(curAngleCal > maxAngleCal){
                maxAngleCalVal = i;
                maxAngleCal = curAngleCal;
            }
        }
        ui->sldTranslate->setValue(maxAngleCalVal);
        ui->btnOpt3->click();
        onConnChange();
    }
    if(connType==1){
        for(int i=50;i<=100;i++){
            ui->btnArgrst->clicked();
            ui->sldTranslate->setValue(i);
            onConnChange();
            ui->btnOpt->click();
            if(std::abs(curPlateRotVal)>acceptableRange)continue;
            ui->sldRotate->setValue(50);
            break;
            //if(!curPilarnValid)break;
        }
        float maxAngleCal = FLT_MIN;
        int maxAngleCalVal = -1;
        int tinit = ui->sldTranslate->value();
        for(int i=tinit;i<tinit+20;i++){
            ui->sldTranslate->setValue(i);
            onConnChange();
            if(curAngleCal > maxAngleCal){
                maxAngleCalVal = i;
                maxAngleCal = curAngleCal;
            }
        }
        ui->sldTranslate->setValue(maxAngleCalVal);
        //ui->btnOpt2->click();
        //ui->btnOpt3->click();
        onConnChange();
    }

}

void MainWindow::on_sldTranslate_valueChanged(int value)
{
    onConnChange();
}

void MainWindow::on_sldTile_valueChanged(int value)
{
    onConnChange();
}

void MainWindow::on_sldRotate_valueChanged(int value)
{
    onConnChange();
}

void MainWindow::on_sldR_valueChanged(int value)
{
    onConnChange();
}


void MainWindow::addConnTar(float r, float r2, QVector3D c, QVector3D n, QVector3D n2, int tar1, int tar2){
    int exist = -1;
    for(int i=0;i<tar1s.size();i++){
        if(tar1s[i]==tar1 && tar2s[i] == tar2){
            exist = i;
            break;
        }
    }
    if(exist>=0){
        ss[exist] = 1;
        rs[exist]=r;
        r2s[exist]=r2;
        ns[exist]=n;
        n2s[exist]=n2;
        cs[exist]=c;
        tar1s[exist]=tar1;
        tar2s[exist]=tar2;
        connTypes[exist] = connType;
    }else{
        ss.push_back(1);
        rs.push_back(r);
        r2s.push_back(r2);
        ns.push_back(n);
        n2s.push_back(n2);
        cs.push_back(c);
        tar1s.push_back(tar1);
        tar2s.push_back(tar2);
        connTypes.push_back(connType);
    }
}

void MainWindow::on_btnApplyOpt_clicked()
{
    QVector3D pointtemp = var.cuttingpoint[var.cutcnt];
    QVector3D pnormtemp = var.cuttingpnorm[var.cutcnt];
    var.cuttingpoint[var.cutcnt] = curC;
    var.cuttingpnorm[var.cutcnt] = curPlateN;
    /*********************************/
    ui->btnDetour->click();
    ui->btnCut->click();
    support.deleteSkelSup();
    /***************************/
    float angleDif = std::abs(ui->sldRotate->value()-50.0f)/50*TileLimit;
    float twistAngle = tan((45.0f-angleDif)/180*M_PI)*180/M_PI;
    twistAngle = curTwistAngle;
    /***************************/
    int tarnum = ui->glMain->getTarnum()-1;
    if(var.IsReverseN[var.cutcnt]){
        ui->glMain->exchangeObj(var.tarObj,tarnum);
        addConnTar(curPilarR, curPlateR, curPilarC,-curPilarN,-curPlateN,tarnum,var.tarObj);
    }else{
        addConnTar(curPilarR, curPlateR, curPilarC,-curPilarN,-curPlateN,var.tarObj,tarnum);
    }
    /*
    int tarnum = ui->glMain->getTarnum()-1;
    ui->glMain->genSpiral(curPilarC,-curPilarN,curPilarR, 1, HDiv, twistAngle, 0.8f);
    int cutnum = ui->glMain->getTarnum()-1;
    support.applyCSG('+', var.tarObj, cutnum);
    ui->glMain->deleteTar(cutnum);
    ui->glMain->genSpiral_withhead(curPilarC,-curPilarN,curPilarR, 1, HDiv, twistAngle, 0.8f, angleDif);
    cutnum = ui->glMain->getTarnum()-1;
    support.applyCSG('-', tarnum, cutnum);
    ui->glMain->deleteTar(cutnum);
    */
    limbNorm[tarnum] = curPilarN;
    /************/
    if(var.skelMgr.path.length()>0){
        support.loadSkelSup();
        ui->showSkel->setChecked(false);
    }
    var.cuttingpoint[var.cutcnt] = pointtemp;
    var.cuttingpnorm[var.cutcnt] = pnormtemp;
}

void MainWindow::on_pushButton_clicked()
{
    /*
    support.stateInit();
    std::string path="./cubetest.obj";
    ui->glMain->load(path.data());
    ui->glMain->viewMgr->Reset();
    ui->glMain->setVis(0,1);
    ui->glMain->reBuffer(0);
    var.tarObj = 0;
    char str[20];
    sprintf(str,"Object %d",ui->glMain->getTarnum());
    for(int i=ui->cmbbObject->count()-1;i>=0;i--){
        ui->cmbbObject->removeItem(i);
    }
    ui->cmbbObject->addItem(QString(str));
    this->setFocus();
    /////////////////////////////////
    QVector3D c = QVector3D(0,0,0);
    QVector3D n = QVector3D(0,1,0);
    float r = 10, l = 50.0f;
    ui->glMain->generateAssitDisc(c, n, r);
    ui->glMain->generateAssitDisc(c, n, r, 1);
    ui->glMain->generateAssitPillar(c, n, r, l);
    ui->glMain->setVis_ass(0, 1);
    ui->glMain->setVis_ass(1, 1);
    ui->glMain->setVis_ass(2, 1);

    onConnChange();
    */
    float tTwistAngle = (float)ui->sbTwistAngle->value()/180.0f*M_PI;
    float DIV = HDiv;
    QVector3D tp1, tp2, tp3;
    tp1 = QVector3D(              1, 0,               0);
    tp2 = QVector3D(cos(tTwistAngle/DIV), sin(tTwistAngle/DIV), 1/DIV);
    tp3 = QVector3D(              0, 1,               0);
    Triangle ttri = Triangle(tp1,tp2,tp3);
    float ttile = 90 - acos(QVector3D::dotProduct(ttri.norm().normalized(), QVector3D(0,0,1)))/M_PI*180;
    std::cout << "tTwistAngle : " << tTwistAngle << std::endl;
    std::cout << "tdot : " << QVector3D::dotProduct(ttri.norm().normalized(), QVector3D(0,0,1)) << std::endl;
    std::cout << "ttile : " << ttile << std::endl;

    /*
    float angleDif = std::abs(ui->sldRotate->value()-50.0f)/50*TileLimit;
    std::cout << "angleDif : " << angleDif << std::endl;
    float twistAngle = tan((45.0f-angleDif)/180*M_PI)*180/M_PI;
    std::cout << "twistAngle : " << twistAngle << std::endl;
    */
}

void MainWindow::on_sbTwistAngle_editingFinished(){
    curTwistAngle = ui->sbTwistAngle->value();
}

void MainWindow::on_btnPrintRot_clicked(){
    limbNorm[0] = limbNorm[3];
    limbNorm[3] = QVector3D(limbNorm[3].x(),-limbNorm[3].y(),limbNorm[3].z());
    limbNorm[8] = limbNorm[9] = QVector3D(0,1,0);
    for(int i=0;i<=9;i++){
        //std::cout << limbNorm[i].x() << " " << limbNorm[i].y() << " " << limbNorm[i].z() << std::endl;
        support.getModel(i)->renewByMatrix();
        support.getModel(i)->rotateFrom(limbNorm[i]);
        ui->glMain->reBuffer(i);
    }
    ui->glMain->assignModel();
}

void MainWindow::on_btnApplyCut_clicked(){
    usedetails5();
}

void MainWindow::on_btnQuickDone_clicked(){
    ui->btnLoadAll->clicked();
    ui->btnCutAll->clicked();
    ui->btnOutputAll->clicked();
}
