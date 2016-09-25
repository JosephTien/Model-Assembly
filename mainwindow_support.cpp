#include<mainwindow.h>

ModelManager* MainWindow::getModel(int tar){
    return &(ui->glMain->viewMgr->modelMgr[tar]);
}

void MainWindow::putStdModel(char * name,QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate){
    char file[30];
    sprintf(file,":/object/%s.obj",name);
    QFile stdObjFile(file);
    ui->glMain->load_rc(&stdObjFile);
    connectTarNum = ui->glMain->getTarnum()-1;
    ui->glMain->setColor(connectTarNum,color.x(),color.y(),color.z());
    getModel(connectTarNum)->ResetModel();
    getModel(connectTarNum)->SetScale(scale.x(),scale.y(),scale.z());
    getModel(connectTarNum)->translate_pure(translate);
    getModel(connectTarNum)->rotateTo(rotate);
    ui->glMain->setVis(connectTarNum,1);
    getModel(connectTarNum)->applyModelMatrix_force();
    ui->glMain->reBuffer(connectTarNum);
}

void MainWindow::applyCSG(char c, int er, int ee){
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
