#include<maintainthread.h>

void MaintainThread::setupUi(Ui::MainWindow * ui){
    (this->ui) = ui;
}
void MaintainThread::setupVar(MaintainVar * var){
    (this->var) = var;
}
void MaintainThread::run(){
    MainWindow_support support(ui, var);
    while(true){
        msleep(300);
        if(var->locked)continue;
        var->lock();
        if(ui->glMain->getTarnum()>0){
            if(support.getModel(var->tarObj)->selecPoints.size()>0){
                var->skelMgr.Reset();
                ui->showSkel->setChecked(false);
                ui->showSkel->setCheckable(false);
                ui->showPlate->setChecked(false);
                ui->showPlate->setCheckable(false);
                if(support.getModel(var->tarObj)->selecPoints.size()==3){
                    ui->showPlate->setCheckable(true);
                }
            }
        }
        var->unlock();
    }
}
