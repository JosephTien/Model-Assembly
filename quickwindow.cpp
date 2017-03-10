#include "quickwindow.h"
#include "ui_quickwindow.h"

QuickWindow::QuickWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuickWindow)
{
    ui->setupUi(this);
    /**/
    Qt::WindowFlags flags = 0;
    setWindowFlags(flags);
    setFixedSize(400, 300);
    /**/
    mui = mainwindow.getUi();
}

QuickWindow::~QuickWindow()
{
    delete ui;
}

void QuickWindow::closeEvent (QCloseEvent *event)
{
    mainwindow.close();
}

void QuickWindow::on_btnMake_clicked()
{
    sendMsg("",true, true);
    mui->btnKinect->click();
}

void QuickWindow::on_btnDeliver_clicked()
{
    QFileInfo file("body.obj");
    if(file.exists()==false){
        sendMsg("尚未製作模型！",true, true);
        return;
    }
    sendMsg("切割中...",false, true);
    cover(true);
    QTimer::singleShot(100, this, SLOT(allStart()));
}
void QuickWindow::allStart(){
    mui->btnQuickDone->click();
    sendMsg("完成",false, true);
    cover(false);
    loginupload();
}
void QuickWindow::cover(bool set){
    if(set){
        ui->labelCover->setGeometry(0,0,400, 210);
    }else{
        ui->labelCover->setGeometry(1000,0,400, 210);
    }
}
void QuickWindow::loginupload(){
    form.setFixedSize(140,140);
    Qt::WindowFlags flags = 0;
    form.setWindowFlags(flags);
    form.show();
}
void QuickWindow::sendMsg(std::string text, bool msg, bool status){
    if(msg)showMsg(text);
    if(status)showStatus(text);
}
void QuickWindow::showMsg(std::string text){
    QMessageBox msgBox;
    msgBox.setText(QString(text.c_str()));
    msgBox.exec();
}
void QuickWindow::showStatus(std::string text){
    ui->labelMsg->setText(QString(text.c_str()));
}
void QuickWindow::on_cbShowMain_stateChanged(int arg1)
{
    if(arg1==0){
        mainwindow.hide();
    }
    else{
        mainwindow.show();
    }
}
void QuickWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_P){
        ui->cbShowMain->toggle();
    }
}
