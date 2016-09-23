#include<mainwindow.h>

ModelManager* MainWindow::getModel(int tar){
    return &(ui->glMain->viewMgr->modelMgr[tar]);
}
