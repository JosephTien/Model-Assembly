#ifndef QUICKWINDOW_H
#define QUICKWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "Form.h"


namespace Ui {
class QuickWindow;
}
/*
class MsgSys : public QThread{
public:
    MsgSys(){}
    Ui::QuickWindow * ui;
    void setupUi(Ui::QuickWindow * ui){(this->ui) = ui;}
    std::string text;bool msg;bool status;
    void send(std::string text, bool msg, bool status){
        this->text = text;
        this->msg = msg;
        this->status = status;
        start();
    }
protected:
    void run(){
        if(msg){
            QMessageBox msgBox;
            msgBox.setText(QString(text.c_str()));
            msgBox.exec();
        }
        if(status){
            this->ui->labelMsg->setText(QString(text.c_str()));
        }
    }
};
*/

class QuickWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuickWindow(QWidget *parent = 0);
    ~QuickWindow();

private slots:
    void keyPressEvent(QKeyEvent *event);

    void closeEvent (QCloseEvent *event);

    void on_btnMake_clicked();

    void on_btnDeliver_clicked();

    void on_cbShowMain_stateChanged(int arg1);

private:
    Ui::QuickWindow *ui;
    Ui::MainWindow *mui;
    MainWindow mainwindow;
    Form form;

    void sendMsg(std::string text, bool msg, bool status);
    void showMsg(std::string text);
    void showStatus(std::string text);
    void cover(bool set);
    void loginupload();
private slots:
    void allStart();

};
#endif // QUICKWINDOW_H
