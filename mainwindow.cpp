#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cmbbSelection->addItem(tr("point 1"));
    ui->cmbbSelection->addItem(tr("point 2"));
    ui->cmbbSelection->addItem(tr("point 3"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_btnLoad_clicked()
{
    if(quickmode){
        //std::string path="../testobj/bunny.obj";
        std::string path="./bunny.obj";
        //std::string path="./cube.obj";
        for(int i=ui->glMain->getTarnum()-1;i>=0;i--){
            ui->glMain->deleteTar(i);
        }
        ui->glMain->load(path.data());
        ui->glMain->viewMgr->Reset();
        ui->glMain->setVis(0,1);
        ui->glMain->reBuffer(0);
        this->setFocus();
        char str[10];
        sprintf(str,"Object %d",ui->glMain->getTarnum());
        for(int i=ui->cmbbSelection->count()-1;i>=0;i--){
            ui->cmbbObject->removeItem(i);
        }
        ui->cmbbObject->addItem(QString(str));
        return;
    }
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
            ui->glMain->viewMgr->Reset();
            ui->glMain->setVis(0,1);
            ui->glMain->reBuffer(0);
            //ui->edition->setChecked(true);
    }
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
    }
    ui->glMain->keyPressEvent(event);
    this->setFocus();
}

void MainWindow::on_btnTest_clicked()
{
    //ui->glMain->generateTest();
    ui->glMain->generateDisc(QVector3D(0,0,0),QVector3D(0,0,1),100.0f, 0);
    this->setFocus();
}

void MainWindow::on_edition_stateChanged(int arg1)
{
    if(arg1 == 0){
        ui->glMain->viewMgr->setControlTar(-1);
        ui->glMain->viewMgr->setControlType(0);
    }else{
         ui->glMain->viewMgr->setControlTar(tarObj);
         ui->glMain->viewMgr->setControlType(1);
         for(int i=ui->glMain->getTarnum_ass()-1;i>=0;i--)ui->glMain->deleteTar_ass(i);
         getModel(tarObj)->selecIdxs.clear();
         getModel(tarObj)->selecPoints.clear();
         getModel(tarObj)->loadColors();
         ui->glMain->reBuffer(tarObj);
    }
    this->setFocus();
}

void MainWindow::on_selection_stateChanged(int arg1)
{
    ui->glMain->setSelectMode(arg1>0);
    if(arg1){
        getModel(tarObj)->applyModelMatrix();
    }else{

    }
    this->setFocus();
}

void MainWindow::on_btnNormalize_clicked()
{
    if(ui->glMain->getTarnum()<1)return;
    getModel(tarObj)->normalize(normalizeVal);
    getModel(tarObj)->applyModelMatrix();
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
    //getModel(tarObj)->regenNormals();

    getModel(tarObj)->calNeighbor();

    //clock_t t1 = clock();
    getModel(tarObj)->calCurvures(1);
    //clock_t t2 = clock();
    //printf("%lf", (t2-t1)/(double)(CLOCKS_PER_SEC));
    //std::cout << std::endl;

    getModel(tarObj)->paintCurvures();
    qmb.close();
    ui->glMain->reBuffer(tarObj);
    this->setFocus();
}

void MainWindow::on_btnDetour_clicked()//tune framework
{
    QVector3D c;
    float r;
    if(ui->glMain->getPlaneParaBySelec(tarObj,c,r)==-1)return;
    int flag = getModel(tarObj)->calDetourByPlane(r);
    if(flag==1){
        getModel(tarObj)->paintDetour();
    }else{
        ui->glMain->deleteTar_ass(assistPlaneIdx);
    }
    getModel(tarObj)->connectorFaceIdxs.clear();
    ui->glMain->reBuffer(tarObj);
}

void MainWindow::on_cmbbSelection_currentIndexChanged(int index)
{
    ui->glMain->curSelecTar=index;
}

void MainWindow::on_btnReset_clicked()
{

    for(int i=ui->glMain->getTarnum()-1;i>tarObj;i++)ui->glMain->deleteTar(i);
    getModel(tarObj)->setColors(0.5f,0.5f,0.5f);
    getModel(tarObj)->refresh();
    getModel(tarObj)->ResetModel();
    ui->glMain->reBuffer(tarObj);
}

void MainWindow::on_showPlate_stateChanged(int arg1)
{
    int tarIdx_ass = 0;
    if(arg1!=0){
        ui->glMain->assistMode = 1;
        ui->glMain->generateAssitDisc(tarObj);
    }else{
        ui->glMain->setVis_ass(tarIdx_ass, 0);
        ui->glMain->assistMode = 0;
    }
    ui->glMain->update();
}

void MainWindow::on_btnCut_clicked()
{
    if(getModel(tarObj)->selecIdxs.size()==0)return;
    char str[20];
    int tarnum = ui->glMain->getTarnum();
    ui->glMain->copyObj(tarObj);

    getModel(tarObj)->cutByDetour(1);
    cgaltool.fillHole(getModel(tarObj)->vertices_ori, getModel(tarObj)->indices);
    ui->glMain->reload("./temp.obj",tarObj);
    getModel(tarObj)->refresh();
    ui->glMain->reBuffer(tarObj);

    getModel(tarnum)->cutByDetour(-1);
    cgaltool.fillHole(getModel(tarnum)->vertices_ori, getModel(tarnum)->indices);
    ui->glMain->reload("./temp.obj",tarnum);
    getModel(tarnum)->refresh();
    ui->glMain->reBuffer(tarnum);

    sprintf(str,"Object %d",tarnum+1);
    ui->cmbbObject->addItem(QString(str));
    ui->glMain->deleteTar_ass(assistPlaneIdx);
}

void MainWindow::on_btnOutput_clicked()
{
    iglMachine.reset();
    iglMachine.put("mainobj", getModel(tarObj)->vertices, getModel(tarObj)->indices);
    char str[20];
    sprintf(str,"test.obj");
    iglMachine.writeFile("mainobj",str);
}

void MainWindow::on_cmbbObject_currentIndexChanged(int index)
{
    for(int i=0;i<ui->glMain->getTarnum();i++){
        if(i==index)ui->glMain->setVis(i,1);
        else ui->glMain->setVis(i,0);
    }
    tarObj = index;
    ui->glMain->tarObj = tarObj;
    ui->glMain->update();
}

void MainWindow::on_btnPushConnector_clicked()
{
    getModel(tarObj)->pushConnect();
    ui->glMain->reBuffer(tarObj);
}

void MainWindow::on_btnPullConnector_clicked()
{
    getModel(tarObj)->pullConnect();
    ui->glMain->reBuffer(tarObj);
}

void MainWindow::on_btnPutConnector_clicked()
{
    if(connectPuted == true){
        ui->glMain->deleteTar(ui->glMain->getTarnum()-1);
    }
    float s = getModel(tarObj)->GetScaleXYZ().x();
    float r = getModel(tarObj)->connectorRadii_ori * s * 1.8;
    QVector3D c = getModel(tarObj)->connectorCenter_ori*s;
    QVector3D n = getModel(tarObj)->connectorNormal_ori;
    if(connectType == 0){
        putStdModel("cylinder10X10",QVector3D(1.0f,0.5f,0.5f),QVector3D(r/100,r/100,0.1f),c,n);

    }else if(connectType == 1){
        r/=1.41421;
        putStdModel("cube10X10",QVector3D(1.0f,0.5f,0.5f),QVector3D(r/100,r/100,0.1f),c,n);
    }
    connectType= (connectType+1)%totalconnectType;
    connectPuted = true;
}

void MainWindow::on_btnSwell_clicked()
{
    if(!connectPuted)return;
    applyCSG('+',tarObj,connectTarNum);
    connectPuted=false;
    ui->glMain->deleteTar(connectTarNum);
}

void MainWindow::on_btnDig_clicked()
{
    if(!connectPuted)return;
    applyCSG('-',tarObj,connectTarNum);
    connectPuted=false;
    ui->glMain->deleteTar(connectTarNum);
}
