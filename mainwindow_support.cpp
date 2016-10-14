#include "mainwindow_support.h"

ModelManager* MainWindow_support::getModel(int tar){
    return &(ui->glMain->viewMgr->modelMgr[tar]);
}

void MainWindow_support::putStdModel(const char * name,QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate){
    char file[30];
    sprintf(file,":/object/%s.obj",name);
    QFile stdObjFile(file);
    ui->glMain->load_rc(&stdObjFile);
    var->connectTarNum = ui->glMain->getTarnum()-1;
    ui->glMain->setColor(var->connectTarNum,color.x(),color.y(),color.z());
    getModel(var->connectTarNum)->ResetModel();
    //getModel(var->connectTarNum)->SetScale(scale.x(),scale.y(),scale.z());
    getModel(var->connectTarNum)->scaleDepend_ori(QVector3D(0,0,0),scale.x(),scale.y(),scale.z());
    getModel(var->connectTarNum)->translate_pure(translate);
    getModel(var->connectTarNum)->rotateTo(rotate);
    ui->glMain->setVis(var->connectTarNum,1);
    getModel(var->connectTarNum)->applyModelMatrix_force();
    ui->glMain->reBuffer(var->connectTarNum);
}

void MainWindow_support::applyCSG(char c, int er, int ee){
    iglMachine.reset();
    iglMachine.put("mainobj", getModel(er)->vertices, getModel(er)->indices);
    iglMachine.put("conn", getModel(ee)->vertices, getModel(ee)->indices);
    iglMachine.command("NEW outcome");
    char str[50];
    sprintf(str,"%c outcome mainobj conn",c);
    iglMachine.command(str);
    iglMachine.get("outcome",getModel(er)->vertices_ori, getModel(er)->indices);
    getModel(er)->ResetModel();
    getModel(er)->refresh();
    ui->glMain->reBuffer(er);
}

void MainWindow_support::drawCircleOnPlane(){
    float r = getModel(var->tarObj)->connectorRadii_ori * 0.95;
    QVector3D c = getModel(var->tarObj)->connectorCenter_ori;
    QVector3D n = getModel(var->tarObj)->connectorNormal_ori;
    int div = ui->edgeNum->value();
    getModel(var->tarObj)->circleOnPlane(c,n,r,div,var->cmpstate);
}

void MainWindow_support::putskelass(){
    std::vector<QVector3D> c;
    std::vector<QVector3D> n;
    var->skelMgr.getPartsInfo(c, n);
    for(int i=0;i<c.size();i++){
        QVector3D s(0.2,0.2,n[i].length()/100);
        putStdModel("cylinder10X10",QVector3D(0.5f,0.5f,1.0f*i/10),s,c[i],n[i]);
        ui->glMain->setVis(ui->glMain->getTarnum()-1,1);
    }
    ui->glMain->update();
}
void MainWindow_support::deleteSkelSup(){
    if(var->skelMgr.loaded){
        for(int i=0;i<10;i++){
            ui->glMain->deleteTar(ui->glMain->getTarnum()-1);
        }
    }
    var->skelMgr.loaded=false;
}
void MainWindow_support::stateInit(){
    ui->sig1->setCheckable(false);
    ui->showSkel->setChecked(false);
    ui->showPlate->setChecked(false);
    ui->showSkel->setCheckable(false);
    ui->showPlate->setCheckable(false);
    deleteSkelSup();
    for(int i=ui->glMain->getTarnum()-1;i>=0;i--){
        ui->glMain->deleteTar(i);
    }
    for(int i=ui->glMain->getTarnum_ass()-1;i>=0;i--){
        ui->glMain->deleteTar_ass(i);
    }

    var->skelMgr.Reset();
    ui->cmbbObject->clear();

}
void MainWindow_support::loadSkelSup(){
    var->skelMgr.load(var->skelMgr.path);
    var->skelMgr.getCutInfo(var->cuttingpoint, var->cuttingpnorm);
    putskelass();//contain "update()"
    ui->showSkel->setCheckable(true);
    ui->showSkel->setChecked(true);
    ui->showPlate->setCheckable(true);
    ui->showPlate->setChecked(false);
    var->skelMgr.loaded = true;
}
void MainWindow_support::fill(int tar){
    cgaltool.fillHole(getModel(tar)->vertices_ori, getModel(tar)->indices);
    iglMachine.reset();iglMachine.readFile("temp","temp.off");
    iglMachine.get("temp",getModel(tar)->vertices_ori, getModel(tar)->indices);
    getModel(tar)->applyModelMatrix_force();
    getModel(tar)->refresh();
}

