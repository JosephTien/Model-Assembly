#include "modelviewer.h"

using namespace std;

ModelViewer::ModelViewer(QWidget *parent) :
    QOpenGLWidget(parent)
{
    viewMgr = ViewManager::GetInstance();
    memset(vis,0,sizeof(int)*tarlimit);
    memset(vis_ass,0,sizeof(int)*tarlimit);
}

int ModelViewer::getTarnum(){
    return tarnum;
}
int ModelViewer::getTarnum_ass(){
    return tarnum_ass;
}

void ModelViewer::deleteTar(int tar){
    makeCurrent();
    tarnum--;
    for(int i=tar;i<tarnum;i++){
        viewMgr->modelMgr[i] = viewMgr->modelMgr[i+1];
        vis[i]=vis[i+1];
        reBuffer(i);
    }
    vertexAttribute[tarnum].Destroy();
    viewMgr->modelMgr[tarnum].Reset();

    update();
}
void ModelViewer::deleteTar_ass(int tar){
    makeCurrent();
    vertexAttribute_ass[tar].Destroy();
    tarnum_ass--;
    for(int i=tar;i<tarnum_ass;i++){
        vertexAttribute_ass[i] = vertexAttribute_ass[i+1];
        vis_ass[i]=vis_ass[i+1];
    }
}

void ModelViewer::setAllVis(int val){
    for(int i=0;i<tarnum;i++){
        vis[i]=val;
    }
}

void ModelViewer::setColor_allrandom(){
    for(int i=0;i<tarnum;i++){
        float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        setColor(i,r1, r2, r3);
    }
}

void ModelViewer::setColor_mySet(){
    for(int i=0;i<tarnum;i++){
        float r1 = i%2 == 0?0.5f:1;
        float r2 = i/2%2 == 0?0.5f:1;
        float r3 = i/4%2 == 0?0.5f:1;

        setColor(i,r1, r2, r3);
        if(i>=8)setColor(i,1, 1, 1);
    }
}

void ModelViewer::setColor(int tar, float r,float g,float b){
    makeCurrent();
    colors.clear();
    for(int i=0;i<(int)viewMgr->modelMgr[tar].vertices_ori.size();i+=3){colors.push_back(r);colors.push_back(g);colors.push_back(b);}
    viewMgr->modelMgr[tar].colors = colors;
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}

void ModelViewer::setColor(int tar, std::vector<float> colors){//no error size handle
    makeCurrent();
    viewMgr->modelMgr[tar].colors = colors;
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);

}
void ModelViewer::setColor_ass(int tar, float r,float g,float b){
    makeCurrent();
    colors.clear();
    for(int i=0;i<(int)viewMgr->modelMgr_ass[tar].vertices_ori.size();i+=3){colors.push_back(r);colors.push_back(g);colors.push_back(b);}
    viewMgr->modelMgr_ass[tar].colors = colors;
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
}
void ModelViewer::reBuffer(int tar){
    makeCurrent();
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}

void ModelViewer::reBuffer_ass(int tar){
    makeCurrent();
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
}

void ModelViewer::reBuffer(){
    makeCurrent();
    for(int tar=0;tar<tarnum;tar++){
        vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    }
}

void ModelViewer::reBuffer_ass(){
    makeCurrent();
    for(int tar=0;tar<tarnum_ass;tar++){
        vector<float> vertices = viewMgr->modelMgr_ass[tar].vertices_ori;
        vector<float> normals = viewMgr->modelMgr_ass[tar].normals;
        vector<unsigned int> indices = viewMgr->modelMgr_ass[tar].indices;
        vector<float> colors = viewMgr->modelMgr_ass[tar].colors;
        vector<float> empty;
        vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    }
    update();
}

void ModelViewer::load_rc(QFile * qfile){
    makeCurrent();
    loadObj_rc(qfile, positions, normals, indices);
    colors.clear();
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
    tarnum++;
    int tar = tarnum-1;
    viewMgr->modelMgr[tar].Reset();
    viewMgr->modelMgr[tar].vertices = positions;
    viewMgr->modelMgr[tar].colors = colors;
    viewMgr->modelMgr[tar].vertices_ori = positions;
    viewMgr->modelMgr[tar].normals = normals;
    viewMgr->modelMgr[tar].indices = indices;
    viewMgr->modelMgr[tar].saveColors();
    vertexAttribute[tar].Create();
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}

void ModelViewer::load_rc_ass(QFile * qfile){
    makeCurrent();
    colors.clear();
    loadObj_rc(qfile, positions, normals, indices);
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
    tarnum_ass++;
    int tar = tarnum_ass-1;
    viewMgr->modelMgr_ass[tar].Reset();
    viewMgr->modelMgr_ass[tar].vertices = positions;
    viewMgr->modelMgr_ass[tar].colors = colors;
    viewMgr->modelMgr_ass[tar].vertices_ori = positions;
    viewMgr->modelMgr_ass[tar].normals = normals;
    viewMgr->modelMgr_ass[tar].indices = indices;
    viewMgr->modelMgr_ass[tar].saveColors();
    vertexAttribute_ass[tar].Create();
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
}

void ModelViewer::loadPillar(){
    makeCurrent();
    colors.clear();
    int tarIdx_ass = 2;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    QFile stdObjFile(":/object/cylinder10X10.obj");
    loadObj_rc(&stdObjFile, positions, normals, indices);
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(1.0f);colors.push_back(0.5f);}
    int tar = tarIdx_ass;
    viewMgr->modelMgr_ass[tar].Reset();
    viewMgr->modelMgr_ass[tar].vertices = positions;
    viewMgr->modelMgr_ass[tar].colors = colors;
    viewMgr->modelMgr_ass[tar].vertices_ori = positions;
    viewMgr->modelMgr_ass[tar].normals = normals;
    viewMgr->modelMgr_ass[tar].indices = indices;
    viewMgr->modelMgr_ass[tar].saveColors();
    vertexAttribute_ass[tar].Create();
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
}


void ModelViewer::load(const char* filename){
    makeCurrent();
    vector<float> empty;
    loadObj(filename, positions, normals, indices);
    colors.clear();
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
    tarnum++;
    int tar = tarnum-1;
    viewMgr->modelMgr[tar].Reset();
    viewMgr->modelMgr[tar].vertices = positions;
    viewMgr->modelMgr[tar].indices = indices;
    viewMgr->modelMgr[tar].vertices_ori = positions;
    viewMgr->modelMgr[tar].normals = normals;
    viewMgr->modelMgr[tar].regenNormals();
    viewMgr->modelMgr[tar].colors = colors;
    viewMgr->modelMgr[tar].saveColors();
    vertexAttribute[tar].Create();
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}
void ModelViewer::reload(const char* filename,int tar){
    makeCurrent();
    if(tar>=tarnum)return;
    loadObj(filename, positions, normals, indices);
    viewMgr->modelMgr[tar].vertices_ori = positions;
    viewMgr->modelMgr[tar].applyModelMatrix_force();
    viewMgr->modelMgr[tar].indices = indices;
    viewMgr->modelMgr[tar].regenNormals();
    viewMgr->modelMgr[tar].setColors(0.5f,0.5f,0.5f);
    viewMgr->modelMgr[tar].saveColors();

    vertexAttribute[tar].Destroy();
    vertexAttribute[tar].Create();
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}

void ModelViewer::copyObj(int tar){
    makeCurrent();
    vertexAttribute[tarnum++].Create();
    viewMgr->modelMgr[tarnum-1] = viewMgr->modelMgr[tar];
    tar = tarnum-1;
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
}

void ModelViewer::exchangeObj(int tar1, int tar2){
    makeCurrent();
    copyObj(tar1);
    viewMgr->modelMgr[tar1] = viewMgr->modelMgr[tar2];
    vertexAttribute[tar1].BufferData(viewMgr->modelMgr[tar2].vertices_ori, viewMgr->modelMgr[tar2].normals, empty, viewMgr->modelMgr[tar2].colors, viewMgr->modelMgr[tar2].indices);
    viewMgr->modelMgr[tar2] = viewMgr->modelMgr[tarnum-1];
    vertexAttribute[tar2].BufferData(viewMgr->modelMgr[tarnum-1].vertices_ori, viewMgr->modelMgr[tarnum-1].normals, empty, viewMgr->modelMgr[tarnum-1].colors, viewMgr->modelMgr[tarnum-1].indices);
    deleteTar(tarnum-1);
}

void ModelViewer::generateCube(QVector3D pup,QVector3D plow){
    makeCurrent();
    float rec[] = {-1,-1,-1,1,-1,-1,-1,1,-1,1,1,-1,-1,-1,1,1,-1,1,-1,1,1,1,1,1};
    float pos[24];
    float e[] = {0};
    for(int i=0;i<24;i+=3){
        pos[i] = rec[i] > 0 ? rec[i]=pup.x():rec[i]=plow.x();
        pos[i+1] = rec[i+1] > 0 ? rec[i+1]=pup.y():rec[i+1]=plow.y();
        pos[i+2] = rec[i+2] > 0 ? rec[i+2]=pup.z():rec[i+2]=plow.z();
    }
    vector<float> empty;
    vector<float> normals(rec,rec+24);
    vector<float> positions(pos,pos+24);
    vector<float> colors = {0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f};
    vector<unsigned int> indices = {0,2,1,2,3,1,3,7,1,7,5,1,5,4,1,4,0,1,0,4,2,4,6,2,6,7,2,7,3,2,5,7,4,7,6,4};
    vertexAttribute[tarnum++].Create();
    vertexAttribute[tarnum-1].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum-1,1);

    viewMgr->modelMgr[tarnum-1].Reset();
    viewMgr->modelMgr[tarnum-1].vertices = positions;
    viewMgr->modelMgr[tarnum-1].vertices_ori = positions;
    viewMgr->modelMgr[tarnum-1].indices = indices;
    viewMgr->modelMgr[tarnum-1].normals = normals;
    viewMgr->modelMgr[tarnum-1].colors = colors;
}

void ModelViewer::generateTest(){
    makeCurrent();
    float rec[] = {-1,-1,-1,1,-1,-1,-1,1,-1,1,1,-1,-1,-1,1,1,-1,1,-1,1,1,1,1,1};
    for(int i=0;i<24;i++){rec[i]*=25;}
    vector<float> empty;
    vector<float> positions(rec, rec + 24);
    vector<float> normals(rec, rec + 24);
    vector<float> colors = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    vector<unsigned int> indices = {0,1,2,2,1,3,3,1,7,7,1,5,5,1,4,4,1,0,0,2,4,4,2,6,6,2,7,7,2,3,5,4,7,7,4,6};

    vertexAttribute[tarnum++].Create();
    vertexAttribute[tarnum-1].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum-1,1);

    for(int i=0;i<24;i++){positions[i]-=25;}
    for(int i=0;i<24;i++){colors[i]/=2;}
    vertexAttribute[tarnum++].Create();
    vertexAttribute[tarnum-1].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum-1,1);
    //vertexAttribute[curtar].Color3f(0.5f, 1.0f, 0.5f);
	update();
    tarnum++;
    viewMgr->modelMgr[tarnum-1].Reset();
    viewMgr->modelMgr[tarnum-1].vertices = positions;
    viewMgr->modelMgr[tarnum-1].indices = indices;
    viewMgr->modelMgr[tarnum-1].vertices_ori = positions;
    viewMgr->modelMgr[tarnum-1].normals = normals;
    viewMgr->modelMgr[tarnum-1].colors = colors;
}
int ModelViewer::getPlaneParaBySelec(int tarObj, QVector3D &c, float &r){
    if(viewMgr->modelMgr[tarObj].selecPoints.size()==3){
        QVector3D v1=viewMgr->modelMgr[tarObj].selecPoints[0];
        QVector3D v2=viewMgr->modelMgr[tarObj].selecPoints[1];
        QVector3D v3=viewMgr->modelMgr[tarObj].selecPoints[2];
        QVector3D center=(v1+v2+v3)/3;
        float radii = ((v1-center).length()+(v2-center).length()+(v3-center).length())/3*2;
        c = center;
        r = radii;
        return 0;
    }
    return -1;
}

void ModelViewer::generateAssitPillar(QVector3D c, QVector3D n, float r, float l){
    loadPillar();
    int tarIdx_ass=2;
    int tar =  tarIdx_ass;
    viewMgr->modelMgr_ass[tar].scaleDepend_ori(QVector3D(0,0,0),r/50,r/50,l/100);
    viewMgr->modelMgr_ass[tar].rotateTo(n);
    viewMgr->modelMgr_ass[tar].translate_pure(c);
    viewMgr->modelMgr_ass[tar].translate_pure(l/2*n.normalized());
    viewMgr->modelMgr_ass[tar].setColors(0.5f, 0.5f, 1.0f);
    viewMgr->modelMgr_ass[tar].applyModelMatrix_force();
    reBuffer_ass(tar);
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
    setVis_ass(tarIdx_ass, assistMode);
}


void ModelViewer::generateAssitDisc(int tarObj, QVector3D &c, float &r){
    int tarIdx_ass=0;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    if(viewMgr->modelMgr[tarObj].selecPoints.size()==3){
        QVector3D v1=viewMgr->modelMgr[tarObj].selecPoints[0];
        QVector3D v2=viewMgr->modelMgr[tarObj].selecPoints[1];
        QVector3D v3=viewMgr->modelMgr[tarObj].selecPoints[2];
        QVector3D center=(v1+v2+v3)/3;
        float radii = ((v1-center).length()+(v2-center).length()+(v3-center).length())/3*2;
        QVector3D platnorm = QVector3D::crossProduct(v1-v3,v2-v3);
        generateDisc(center,platnorm,radii,tarIdx_ass);
        setVis_ass(tarIdx_ass, assistMode);
        c = center;
        r = radii;
    }
}
void ModelViewer::generateAssitDisc(QVector3D center, QVector3D platnorm, float radii){
    int tarIdx_ass=0;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    generateDisc(center,platnorm,radii,tarIdx_ass);
    setVis_ass(tarIdx_ass, assistMode);
    viewMgr->modelMgr_ass[tarIdx_ass].setColors(0.5f, 1.0f, 0.5f);
    reBuffer_ass(tarIdx_ass);
}
void ModelViewer::generateAssitDisc(QVector3D center, QVector3D platnorm, float radii, int tar){
    int tarIdx_ass=tar;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    generateDisc(center,platnorm,radii,tarIdx_ass);
    setVis_ass(tarIdx_ass, assistMode);
    viewMgr->modelMgr_ass[tarIdx_ass].setColors(1.0f, 0.5f, 0.5f);
    reBuffer_ass(tarIdx_ass);
}

void ModelViewer::generateAssitDisc(int tarObj){
    int tarIdx_ass=0;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    if(viewMgr->modelMgr[tarObj].selecPoints.size()==3){
        QVector3D v1=viewMgr->modelMgr[tarObj].selecPoints[0];
        QVector3D v2=viewMgr->modelMgr[tarObj].selecPoints[1];
        QVector3D v3=viewMgr->modelMgr[tarObj].selecPoints[2];
        QVector3D center=(v1+v2+v3)/3;
        float radii = ((v1-center).length()+(v2-center).length()+(v3-center).length())/3*2;
        QVector3D platnorm = QVector3D::crossProduct(v1-v3,v2-v3);
        generateDisc(center,platnorm,radii,tarIdx_ass);
        setVis_ass(tarIdx_ass, assistMode);
    }
}
/*
void ModelViewer::generateAssitDisc(int tarObj){
    int tarIdx_ass=0;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    if(viewMgr->modelMgr[tarObj].selecPoints.size()==3){
        QVector3D v1=viewMgr->modelMgr[tarObj].selecPoints[0];
        QVector3D v2=viewMgr->modelMgr[tarObj].selecPoints[1];
        QVector3D v3=viewMgr->modelMgr[tarObj].selecPoints[2];
        QVector3D center=(v1+v2+v3)/3;
        float radii = ((v1-center).length()+(v2-center).length()+(v3-center).length())/3*2;
        QVector3D platnorm = QVector3D::crossProduct(v1-v3,v2-v3);
        generateDisc(center,platnorm,radii,tarIdx_ass);
        setVis_ass(tarIdx_ass, assistMode);
    }
}
*/
//int DIV = 100;

void ModelViewer::genTrack(QVector3D center, QVector3D platenorm, float radii, bool minerr){

    float rate = radii / 1.0f;
    //std::cout << "rate : " << rate << std::endl;

    float h = radii/2.0f, r = radii/1.3f, r2 = radii/1.1f, l = r2*rate;
    if(minerr){
        r-=0.5f;
        r2-=0.5f;
    }
    platenorm.normalize();
    platenorm*=-1;
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));
    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
    vector<float> normals;

    vertices.push_back(QVector3D(l, 0, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(0, r, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(-l, 0, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(0, -r, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(l, 0, h)*rotationMat+center);
    vertices.push_back(QVector3D(0, r2, h)*rotationMat+center);
    vertices.push_back(QVector3D(-l, 0, h)*rotationMat+center);
    vertices.push_back(QVector3D(0, -r2, h)*rotationMat+center);

    indices.push_back(0);indices.push_back(2);indices.push_back(1);
    indices.push_back(0);indices.push_back(3);indices.push_back(2);
    indices.push_back(4);indices.push_back(5);indices.push_back(6);
    indices.push_back(4);indices.push_back(6);indices.push_back(7);

    indices.push_back(0);indices.push_back(1);indices.push_back(5);
    indices.push_back(1);indices.push_back(2);indices.push_back(6);
    indices.push_back(2);indices.push_back(3);indices.push_back(7);
    indices.push_back(3);indices.push_back(0);indices.push_back(4);

    indices.push_back(0);indices.push_back(5);indices.push_back(4);
    indices.push_back(1);indices.push_back(6);indices.push_back(5);
    indices.push_back(2);indices.push_back(7);indices.push_back(6);
    indices.push_back(3);indices.push_back(4);indices.push_back(7);

    /**********************************************/
    vector<float> empty;
    vector<float> positions;
    vector<float> colors;
    QVector3D c;
    for(int i=0;i<vertices.size();i++){
        c+=vertices[i];
    }
    c/=vertices.size();
    for(int i=0;i<vertices.size();i++){
        positions.push_back(vertices[i].x());
        positions.push_back(vertices[i].y());
        positions.push_back(vertices[i].z());
        normals.push_back(vertices[i].x()-c.x());
        normals.push_back(vertices[i].y()-c.y());
        normals.push_back(vertices[i].z()-c.z());
        colors.push_back(0.5f);
        colors.push_back(0.5f);
        colors.push_back(1);
    }
    makeCurrent();
    vertexAttribute[tarnum].Destroy();
    vertexAttribute[tarnum].Create();
    vertexAttribute[tarnum].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum,1);
    update();
    viewMgr->modelMgr[tarnum].Reset();
    viewMgr->modelMgr[tarnum].vertices = positions;
    viewMgr->modelMgr[tarnum].indices = indices;
    viewMgr->modelMgr[tarnum].vertices_ori = positions;
    viewMgr->modelMgr[tarnum].normals = normals;
    viewMgr->modelMgr[tarnum].colors = colors;
    tarnum++;
}

void ModelViewer::genSpiralCav(QVector3D center, QVector3D platenorm, float radii,float hrate){

    float h = radii * hrate, r = radii;
    platenorm.normalize();
    platenorm*=-1;
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));
    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
    vector<float> normals;

    vertices.push_back(QVector3D(radii*2, -radii/4, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(radii*2, radii/4, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(-radii*2, radii/4, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(-radii*2, -radii/4, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(radii*2, -radii/4, h*2)*rotationMat+center);
    vertices.push_back(QVector3D(radii*2, radii/4, h*2)*rotationMat+center);
    vertices.push_back(QVector3D(-radii*2, radii/4, h*2)*rotationMat+center);
    vertices.push_back(QVector3D(-radii*2, -radii/4, h*2)*rotationMat+center);

    indices.push_back(0);indices.push_back(2);indices.push_back(1);
    indices.push_back(0);indices.push_back(3);indices.push_back(2);
    indices.push_back(4);indices.push_back(5);indices.push_back(6);
    indices.push_back(4);indices.push_back(6);indices.push_back(7);

    indices.push_back(0);indices.push_back(1);indices.push_back(5);
    indices.push_back(1);indices.push_back(2);indices.push_back(6);
    indices.push_back(2);indices.push_back(3);indices.push_back(7);
    indices.push_back(3);indices.push_back(0);indices.push_back(4);

    indices.push_back(0);indices.push_back(5);indices.push_back(4);
    indices.push_back(1);indices.push_back(6);indices.push_back(5);
    indices.push_back(2);indices.push_back(7);indices.push_back(6);
    indices.push_back(3);indices.push_back(4);indices.push_back(7);

    /**********************************************/
    vector<float> empty;
    vector<float> positions;
    vector<float> colors;
    QVector3D c;
    for(int i=0;i<vertices.size();i++){
        c+=vertices[i];
    }
    c/=vertices.size();
    for(int i=0;i<vertices.size();i++){
        positions.push_back(vertices[i].x());
        positions.push_back(vertices[i].y());
        positions.push_back(vertices[i].z());
        normals.push_back(vertices[i].x()-c.x());
        normals.push_back(vertices[i].y()-c.y());
        normals.push_back(vertices[i].z()-c.z());
        colors.push_back(0.5f);
        colors.push_back(0.5f);
        colors.push_back(1);
    }
    makeCurrent();
    vertexAttribute[tarnum].Destroy();
    vertexAttribute[tarnum].Create();
    vertexAttribute[tarnum].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum,1);
    update();
    viewMgr->modelMgr[tarnum].Reset();
    viewMgr->modelMgr[tarnum].vertices = positions;
    viewMgr->modelMgr[tarnum].indices = indices;
    viewMgr->modelMgr[tarnum].vertices_ori = positions;
    viewMgr->modelMgr[tarnum].normals = normals;
    viewMgr->modelMgr[tarnum].colors = colors;
    tarnum++;
}

int cornNum = 4;
void ModelViewer::genSpiral_withhead(QVector3D center, QVector3D platenorm, float radii,float hrate, int div, float angle, float dec, float decu, float headAngle){

    float h = radii * hrate, r = radii;
    platenorm.normalize();
    platenorm*=-1;
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));

    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
    vector<float> normals;
    for(int i=0;i<=div;i++){
        //float rr = std::abs((float)(i-(div/2))/(div/2));
        float rr = (float)(i-(div))/div;
        r = radii * (1-((1-dec)* rr));
        float curangle = (angle / div * i) / 360 * 2 *M_PI;
        for(int j=0;j<cornNum;j++){
            vertices.push_back(QVector3D(r*cos(curangle+M_PI*2*j/cornNum), r*sin(curangle+M_PI*2*j/cornNum), h / div * i));
        }
        /*
        vertices.push_back(QVector3D(r*cos(curangle), r*sin(curangle), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI/2), r*sin(curangle+M_PI/2), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI), r*sin(curangle+M_PI), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI/2*3), r*sin(curangle+M_PI/2*3), h / div * i));
        */
    }
    for(int i=0;i<cornNum;i++){
        vertices[i]-=QVector3D(0,0,0.01f);
    }
    /*
    vertices[0]-=QVector3D(0,0,0.01f);
    vertices[1]-=QVector3D(0,0,0.01f);
    vertices[2]-=QVector3D(0,0,0.01f);
    vertices[3]-=QVector3D(0,0,0.01f);
    */
    float hp = (tan(M_PI/4 + (headAngle*M_PI/180)) * r * dec) + h;
    vertices.push_back(QVector3D(0,0,hp)*rotationMat+center);

    div+=1;
    for(int i=0;i<div;i++){
        for(int j=0;j<cornNum;j++){
            vertices[i*cornNum+j] = vertices[i*cornNum+j]*rotationMat+center;
        }
        /*
        vertices[i*cornum]   = vertices[i*4]*rotationMat+center;
        vertices[i*4+1] =  vertices[i*4+1]*rotationMat+center;
        vertices[i*4+2] =  vertices[i*4+2]*rotationMat+center;
        vertices[i*4+3] =  vertices[i*4+3]*rotationMat+center;
        */
    }

    for(int i=0;i<div-1;i++){
        for(int j=0;j<cornNum;j++){
            indices.push_back(i*cornNum+j);indices.push_back(i*cornNum+(1+j)%cornNum);indices.push_back((i+1)*cornNum+j);
            indices.push_back((i+1)*cornNum+(1+j)%cornNum);indices.push_back((i+1)*cornNum+j);indices.push_back(i*cornNum+(1+j)%cornNum);
        }
        /*
        indices.push_back(i*4);indices.push_back(i*4+1);indices.push_back((i+1)*4);
        indices.push_back(i*4+1);indices.push_back(i*4+2);indices.push_back((i+1)*4+1);
        indices.push_back(i*4+2);indices.push_back(i*4+3);indices.push_back((i+1)*4+2);
        indices.push_back(i*4+3);indices.push_back(i*4);indices.push_back((i+1)*4+3);
        indices.push_back((i+1)*4+1);indices.push_back((i+1)*4);indices.push_back(i*4+1);
        indices.push_back((i+1)*4+2);indices.push_back((i+1)*4+1);indices.push_back(i*4+2);
        indices.push_back((i+1)*4+3);indices.push_back((i+1)*4+2);indices.push_back(i*4+3);
        indices.push_back((i+1)*4);indices.push_back((i+1)*4+3);indices.push_back(i*4);
        */
    }
    for(int i=0;i<cornNum-2;i++){
        indices.push_back(0);indices.push_back(i+2);indices.push_back(i+1);
    }
    for(int i=0;i<cornNum;i++){
        indices.push_back(div*cornNum-cornNum+i);indices.push_back(div*cornNum-cornNum+(1+i)%cornNum);indices.push_back(div*cornNum);
    }
    /*
    indices.push_back(0);indices.push_back(3);indices.push_back(1);
    indices.push_back(1);indices.push_back(3);indices.push_back(2);
    indices.push_back(div*4-4);indices.push_back(div*4-3);indices.push_back(div*4);
    indices.push_back(div*4-3);indices.push_back(div*4-2);indices.push_back(div*4);
    indices.push_back(div*4-2);indices.push_back(div*4-1);indices.push_back(div*4);
    indices.push_back(div*4-1);indices.push_back(div*4-4);indices.push_back(div*4);
    */
    /****************************************/
    vector<float> empty;
    vector<float> positions;
    vector<float> colors;
    QVector3D c;
    for(int i=0;i<vertices.size();i++){
        c+=vertices[i];
    }
    c/=vertices.size();
    for(int i=0;i<vertices.size();i++){
        positions.push_back(vertices[i].x());
        positions.push_back(vertices[i].y());
        positions.push_back(vertices[i].z());
        normals.push_back(vertices[i].x()-c.x());
        normals.push_back(vertices[i].y()-c.y());
        normals.push_back(vertices[i].z()-c.z());
        colors.push_back(0.5f);
        colors.push_back(0.5f);
        colors.push_back(1);
    }
    makeCurrent();
    vertexAttribute[tarnum].Destroy();
    vertexAttribute[tarnum].Create();
    vertexAttribute[tarnum].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum,1);
    //vertexAttribute[curtar].Color3f(0.5f, 1.0f, 0.5f);
    update();
    viewMgr->modelMgr[tarnum].Reset();
    viewMgr->modelMgr[tarnum].vertices = positions;
    viewMgr->modelMgr[tarnum].indices = indices;
    viewMgr->modelMgr[tarnum].vertices_ori = positions;
    viewMgr->modelMgr[tarnum].normals = normals;
    viewMgr->modelMgr[tarnum].colors = colors;
    tarnum++;
}

void ModelViewer::genSpiral(QVector3D center, QVector3D platenorm, float radii,float hrate, int div, float angle, float dec, float decu){
    float innerFix = 0.25f;
    //float angle = 90;
    float h = radii * hrate, r = radii;
    //int div = DIV;
    platenorm.normalize();
    platenorm*=-1;
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));

    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
    vector<float> normals;
    for(int i=0;i<=div;i++){
        //float rr = std::abs((float)(i-(div/2))/(div/2));
        float rr = (float)(i-(div))/div;
        r = radii * (1-((1-dec)* rr)) - innerFix;
        float curangle = (angle / div * i) / 360 * 2 *M_PI;
        for(int j=0;j<cornNum;j++){
            vertices.push_back(QVector3D(r*cos(curangle+M_PI*2*j/cornNum), r*sin(curangle+M_PI*2*j/cornNum), h / div * i));
        }
        /*
        vertices.push_back(QVector3D(r*cos(curangle), r*sin(curangle), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI/2), r*sin(curangle+M_PI/2), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI), r*sin(curangle+M_PI), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI/2*3), r*sin(curangle+M_PI/2*3), h / div * i));
        */
    }
    for(int i=0;i<cornNum;i++){
        vertices[i]-=QVector3D(0,0,0.01f);
    }
    /*
    vertices[0]-=QVector3D(0,0,0.01f);
    vertices[1]-=QVector3D(0,0,0.01f);
    vertices[2]-=QVector3D(0,0,0.01f);
    vertices[3]-=QVector3D(0,0,0.01f);
    */

    div+=1;
    for(int i=0;i<div;i++){
        for(int j=0;j<cornNum;j++){
            vertices[i*cornNum+j] = vertices[i*cornNum+j]*rotationMat+center;
        }
        /*
        vertices[i*4]   = vertices[i*4]*rotationMat+center;
        vertices[i*4+1] =  vertices[i*4+1]*rotationMat+center;
        vertices[i*4+2] =  vertices[i*4+2]*rotationMat+center;
        vertices[i*4+3] =  vertices[i*4+3]*rotationMat+center;
        */
    }
    for(int i=0;i<div-1;i++){
        for(int j=0;j<cornNum;j++){
            indices.push_back(i*cornNum+j);indices.push_back(i*cornNum+(1+j)%cornNum);indices.push_back((i+1)*cornNum+j);
            indices.push_back((i+1)*cornNum+(1+j)%cornNum);indices.push_back((i+1)*cornNum+j);indices.push_back(i*cornNum+(1+j)%cornNum);
        }
        /*
        indices.push_back(i*4);indices.push_back(i*4+1);indices.push_back((i+1)*4);
        indices.push_back(i*4+1);indices.push_back(i*4+2);indices.push_back((i+1)*4+1);
        indices.push_back(i*4+2);indices.push_back(i*4+3);indices.push_back((i+1)*4+2);
        indices.push_back(i*4+3);indices.push_back(i*4);indices.push_back((i+1)*4+3);
        indices.push_back((i+1)*4+1);indices.push_back((i+1)*4);indices.push_back(i*4+1);
        indices.push_back((i+1)*4+2);indices.push_back((i+1)*4+1);indices.push_back(i*4+2);
        indices.push_back((i+1)*4+3);indices.push_back((i+1)*4+2);indices.push_back(i*4+3);
        indices.push_back((i+1)*4);indices.push_back((i+1)*4+3);indices.push_back(i*4);
        */
    }
    for(int i=0;i<cornNum-2;i++){
        indices.push_back(0);indices.push_back(i+2);indices.push_back(i+1);
    }
    for(int i=0;i<cornNum-2;i++){
        indices.push_back(div*cornNum-cornNum);indices.push_back(div*cornNum-cornNum+1+i);indices.push_back(div*cornNum-cornNum+2+i);
    }
    /*
    indices.push_back(0);indices.push_back(3);indices.push_back(1);
    indices.push_back(1);indices.push_back(3);indices.push_back(2);
    indices.push_back(div*4-4);indices.push_back(div*4-3);indices.push_back(div*4-1);
    indices.push_back(div*4-1);indices.push_back(div*4-3);indices.push_back(div*4-2);
    */
    /****************************************/
    vector<float> empty;
    vector<float> positions;
    vector<float> colors;
    QVector3D c;
    for(int i=0;i<vertices.size();i++){
        c+=vertices[i];
    }
    c/=vertices.size();
    for(int i=0;i<vertices.size();i++){
        positions.push_back(vertices[i].x());
        positions.push_back(vertices[i].y());
        positions.push_back(vertices[i].z());
        normals.push_back(vertices[i].x()-c.x());
        normals.push_back(vertices[i].y()-c.y());
        normals.push_back(vertices[i].z()-c.z());
        colors.push_back(0.5f);
        colors.push_back(0.5f);
        colors.push_back(1);
    }
    makeCurrent();
    vertexAttribute[tarnum].Destroy();
    vertexAttribute[tarnum].Create();
    vertexAttribute[tarnum].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum,1);
    //vertexAttribute[curtar].Color3f(0.5f, 1.0f, 0.5f);
    update();
    viewMgr->modelMgr[tarnum].Reset();
    viewMgr->modelMgr[tarnum].vertices = positions;
    viewMgr->modelMgr[tarnum].indices = indices;
    viewMgr->modelMgr[tarnum].vertices_ori = positions;
    viewMgr->modelMgr[tarnum].normals = normals;
    viewMgr->modelMgr[tarnum].colors = colors;
    tarnum++;
}

void ModelViewer::assignModel(){
    for(int i=0;i<tarnum;i++){
        viewMgr->modelMgr[i].renewByMatrix();
        QVector3D m = viewMgr->modelMgr[i].minZVertice_ori();
        QVector3D a = viewMgr->modelMgr[i].averageVertice_ori();
        viewMgr->modelMgr[i].translate_pure(-QVector3D(a.x()-200+40*i,a.y(),m.z()));
        reBuffer(i);
        viewMgr->modelMgr[i].applyModelMatrix_force();
    }
}

void ModelViewer::generateDisc(QVector3D center, QVector3D platenorm, float radii, int tarIdx_ass){
    makeCurrent();
    vector<float> empty;
    vector<float> positions;
    vector<float> normals;
    vector<float> colors;
    vector<unsigned int> indices;
    platenorm.normalize();
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));
    positions.push_back(center.x());positions.push_back(center.y());positions.push_back(center.z());
    colors.push_back(0);colors.push_back(1);colors.push_back(0);
    int divnum=24;
    for(int i=0;i<divnum;i++){
        QVector3D curpoint = (QVector4D(radii*cos(2*M_PI*i/divnum), radii*sin(2*M_PI*i/divnum), 0 ,1)*rotationMat).toVector3DAffine();
        curpoint += center;
        positions.push_back(curpoint.x());positions.push_back(curpoint.y());positions.push_back(curpoint.z());
        colors.push_back(0);colors.push_back(1);colors.push_back(0);
        indices.push_back(0);indices.push_back(i+1);
        if(i+2<divnum+1)indices.push_back(i+2);else indices.push_back(1);
    }

    vertexAttribute_ass[tarIdx_ass].Destroy();
    vertexAttribute_ass[tarIdx_ass].Create();
    vertexAttribute_ass[tarIdx_ass].BufferData(positions, normals, empty, colors, indices);
    setVis_ass(tarIdx_ass,1);
    //vertexAttribute[curtar].Color3f(0.5f, 1.0f, 0.5f);
    update();
    viewMgr->modelMgr_ass[tarIdx_ass].Reset();
    viewMgr->modelMgr_ass[tarIdx_ass].vertices = positions;
    viewMgr->modelMgr_ass[tarIdx_ass].indices = indices;
    viewMgr->modelMgr_ass[tarIdx_ass].vertices_ori = positions;
    viewMgr->modelMgr_ass[tarIdx_ass].normals = normals;
    viewMgr->modelMgr_ass[tarIdx_ass].colors = colors;
}

void ModelViewer::initializeGL()
{

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    standardShader.Create();
    //generateTest();

}

void ModelViewer::paintGL()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //standardShader.SetRenderMode(RENDER_COLORED);
    standardShader.SetRenderMode(RENDER_PHONG_COLORED);
    for(int i=0;i<tarnum;i++){
        standardShader.SetLightDirection(QVector3D(0,0,1)*viewMgr->GetViewRotationMatrix());
        standardShader.SetModelMatrix(viewMgr->GetModelMatrix(i));
        standardShader.SetMVPMatrix(viewMgr->GetModelViewProjectionMatrix(viewportAspect, i));
        if(vis[i]){
            standardShader.Draw(GL_TRIANGLES, vertexAttribute[i]);
        }
    }
    for(int i=0;i<tarnum_ass;i++){
        standardShader.SetLightDirection(QVector3D(0,0,1)*viewMgr->GetViewRotationMatrix());
        standardShader.SetModelMatrix(viewMgr->GetModelMatrix_ass(i));
        standardShader.SetMVPMatrix(viewMgr->GetModelViewProjectionMatrix_ass(viewportAspect, i));
        if(vis_ass[i]){
            standardShader.Draw(GL_TRIANGLES, vertexAttribute_ass[i]);
        }
    }
}

void ModelViewer::resizeGL(int w, int h)
{
    dpr = QPaintDevice::devicePixelRatio();

    viewportSize = QSize(w*dpr, h*dpr);
    viewportAspect = (float)w / (float)h;
}


QVector3D ModelViewer::rayIntersec(Ray3 ray,std::vector<float> verts, std::vector<unsigned int> indices, int &resultnum){
    std::vector<QVector3D> vertices;
    for(int i=0;i<(int)verts.size();i+=3)vertices.push_back(QVector3D(verts[i],verts[i+1],verts[i+2]));
    std::vector<QVector3D> candidateVert;
    std::vector<unsigned int> candidateIdx;
    for(int i=0;i<(int)indices.size();i+=3){
        QVector3D p1 = vertices[indices[i]];
        QVector3D p2 = vertices[indices[i+1]];
        QVector3D p3 = vertices[indices[i+2]];
        QVector3D n  = QVector3D::normal((p2-p1),(p3-p1));
        QVector3D q  = ray.origin;
        QVector3D v  = ray.direction;
        QVector3D r  = (p1-q);

        if(QVector3D::dotProduct(v,n)==0)continue;
        float t = QVector3D::dotProduct(r,n)/QVector3D::dotProduct(v,n);
        if(t<0)continue;
        QVector3D c = q + t*v;
        c = c - QVector3D::dotProduct((c-p1), n.normalized()) * n.normalized();

        float d1 = QVector3D::dotProduct(QVector3D::crossProduct(p1-c,p2-c),n);
        float d2 = QVector3D::dotProduct(QVector3D::crossProduct(p2-c,p3-c),n);
        float d3 = QVector3D::dotProduct(QVector3D::crossProduct(p3-c,p1-c),n);
        if((d1>=0&&d2>=0&&d3>=0)||(d1<=0&&d2<=0&&d3<=0)){
            candidateVert.push_back(c);
            candidateIdx.push_back(i/3);
        }
    }
    resultnum = (int)candidateVert.size();
    QVector3D bestCandidateVert;
    unsigned int bestCandidateIdx;
    float bestCandidateDepth = FLT_MAX;
    for(int i=0;i<(int)candidateVert.size();i++){
        float dis = (ray.origin-candidateVert[i]).length();
        if(dis<bestCandidateDepth){
            bestCandidateDepth = dis;
            bestCandidateVert = candidateVert[i];
            bestCandidateIdx = candidateIdx[i];
        }
    }
    return bestCandidateVert;
}

void ModelViewer::mousePressEvent(QMouseEvent *event)
{
    if(selectMode){
        if(viewMgr->getControlType()==0){
            winw=this->geometry().width();
            winh=this->geometry().height();
            float planeh = (viewMgr->eyelookdis-viewMgr->clipdis)*2*tan(viewMgr->perspangle*viewMgr->getZoom()/2 * M_PI / 180.0 );
            float planew = planeh * winw / winh;
            Ray3 ray;
            ray.origin.setX( (event->x()-winw/2)        *planew/winw );
            ray.origin.setY( (-(event->y()-winh/2))     *planeh/winh );
            ray.origin.setZ( viewMgr->eyeLookPosition.z() + viewMgr->clipdis );
            ray.direction = ray.origin - viewMgr->eyePosition;
            QMatrix4x4 mat = viewMgr->GetViewTranslationMatrix().inverted() * viewMgr->GetViewRotationMatrix().inverted();
            ray.origin = mat * ray.origin;
            mat = viewMgr->GetViewRotationMatrix().inverted();
            ray.direction = mat * ray.direction;

            viewMgr->modelMgr[tarObj].applyModelMatrix();
            int internum;
            QVector3D ri = rayIntersec(ray, viewMgr->modelMgr[tarObj].vertices, viewMgr->modelMgr[tarObj].indices, internum);
            if(internum>0){
                for(int i=(int)viewMgr->modelMgr[tarObj].selecPoints.size();i<=curSelecTar;i++){
                    viewMgr->modelMgr[tarObj].selecPoints.push_back(QVector3D(viewMgr->modelMgr[tarObj].vertices[0],viewMgr->modelMgr[tarObj].vertices[1],viewMgr->modelMgr[tarObj].vertices[2]));
                    viewMgr->modelMgr[tarObj].selecIdxs.push_back(0);
                }
                viewMgr->modelMgr[tarObj].selecPoints[curSelecTar] = ri;
                viewMgr->modelMgr[tarObj].fitSelecIdxs(curSelecTar);
                viewMgr->modelMgr[tarObj].loadColors();
                viewMgr->modelMgr[tarObj].paintSelecIdxs();
                reBuffer(tarObj);
                //viewMgr->modelMgr[tarObj].loadColors();
                generateAssitDisc(tarObj);
            }
        }
    }else{
        viewMgr->mousePressEvent(event);
    }
}
void ModelViewer::mouseReleaseEvent(QMouseEvent *event)
{
    viewMgr->mouseReleaseEvent(event);

    update();
}

void ModelViewer::mouseMoveEvent(QMouseEvent *event)
{
    viewMgr->mouseMoveEvent(event);
    update();
}

void ModelViewer::wheelEvent(QWheelEvent *event)
{
    viewMgr->wheelEvent(event);
    update();
}
void ModelViewer::keyReleaseEvent(QKeyEvent *){//QKeyEvent *event
    viewMgr->setFixAxis(QVector2D(1,1));
    this->setFocus();
}
void ModelViewer::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Z){
         viewMgr->setFixAxis(QVector2D(1,0));
    }else if(event->key() == Qt::Key_X){
        viewMgr->setFixAxis(QVector2D(0,1));
    }else if(event->key() == Qt::Key_Q){
         viewMgr->ResetView();
    }else if(event->key() == Qt::Key_W){
        viewMgr->ResetModel();
        //viewMgr->modelMgr[0].normalize(50.0f);
    }else if(event->key() == Qt::Key_Left){
        viewMgr->keyPressEvent(event);
    }else if(event->key() == Qt::Key_Right){
        viewMgr->keyPressEvent(event);
    }else if(event->key() == Qt::Key_Up){
        viewMgr->keyPressEvent(event);
    }else if(event->key() == Qt::Key_Down){
        viewMgr->keyPressEvent(event);
    }
    update();
}
