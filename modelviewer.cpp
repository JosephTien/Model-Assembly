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
    vertexAttribute[tar].Destroy();
    tarnum--;
    for(int i=tar;i<tarnum;i++){
        vertexAttribute[i] = vertexAttribute[i+1];
        vis[i]=vis[i+1];
    }
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

void ModelViewer::setColor(int tar, float r,float g,float b){
    makeCurrent();
    vector<float> vertices = viewMgr->modelMgr[tar].vertices_ori;
    vector<float> normals = viewMgr->modelMgr[tar].normals;
    vector<unsigned int> indices = viewMgr->modelMgr[tar].indices;
    vector<float> empty;
    vector<float> colors;
    for(int i=0;i<(int)vertices.size();i+=3){colors.push_back(r);colors.push_back(g);colors.push_back(b);}
    vertexAttribute[tar].BufferData(vertices, normals, empty, colors, indices);
    viewMgr->modelMgr[tar].colors = colors;
}

void ModelViewer::setColor(int tar, std::vector<float> colors){//no error size handle
    makeCurrent();
    vector<float> vertices = viewMgr->modelMgr[tar].vertices_ori;
    vector<float> normals = viewMgr->modelMgr[tar].normals;
    vector<unsigned int> indices = viewMgr->modelMgr[tar].indices;
    vector<float> empty;
    vertexAttribute[tar].BufferData(vertices, normals, empty, colors, indices);
    viewMgr->modelMgr[tar].colors = colors;
}

void ModelViewer::reBuffer(int tar){
    makeCurrent();
    vector<float> vertices = viewMgr->modelMgr[tar].vertices_ori;
    vector<float> normals = viewMgr->modelMgr[tar].normals;
    vector<unsigned int> indices = viewMgr->modelMgr[tar].indices;
    vector<float> colors = viewMgr->modelMgr[tar].colors;
    vector<float> empty;
    vertexAttribute[tar].BufferData(vertices, normals, empty, colors, indices);
    update();
}

void ModelViewer::reBuffer_ass(int tar){
    makeCurrent();
    vector<float> vertices = viewMgr->modelMgr_ass[tar].vertices_ori;
    vector<float> normals = viewMgr->modelMgr_ass[tar].normals;
    vector<unsigned int> indices = viewMgr->modelMgr_ass[tar].indices;
    vector<float> colors = viewMgr->modelMgr_ass[tar].colors;
    vector<float> empty;
    vertexAttribute_ass[tar].BufferData(vertices, normals, empty, colors, indices);
}

void ModelViewer::reBuffer(){
    makeCurrent();
    for(int tar=0;tar<tarnum;tar++){
        vector<float> vertices = viewMgr->modelMgr[tar].vertices_ori;
        vector<float> normals = viewMgr->modelMgr[tar].normals;
        vector<unsigned int> indices = viewMgr->modelMgr[tar].indices;
        vector<float> colors = viewMgr->modelMgr[tar].colors;
        vector<float> empty;
        vertexAttribute[tar].BufferData(vertices, normals, empty, colors, indices);
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
        vertexAttribute_ass[tar].BufferData(vertices, normals, empty, colors, indices);
    }
}

void ModelViewer::load_rc(QFile * qfile){
    makeCurrent();
    vertexAttribute[tarnum++].Create();
    vector<float> empty;
    vector<float> colors;
    loadObj_rc(qfile, positions, normals, indices);
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
//    std::cout<<"load     : " << filename << std::endl;
//    std::cout<<"face num : " << indices.size()/3 << std::endl;
    vertexAttribute[tarnum-1].BufferData(positions, normals, empty, colors, indices);
    update();
    viewMgr->modelMgr[tarnum-1].Reset();
    viewMgr->modelMgr[tarnum-1].vertices = positions;
    viewMgr->modelMgr[tarnum-1].colors = colors;
    viewMgr->modelMgr[tarnum-1].vertices_ori = positions;
    viewMgr->modelMgr[tarnum-1].normals = normals;
    viewMgr->modelMgr[tarnum-1].indices = indices;
    viewMgr->modelMgr[tarnum-1].saveColors();

}

void ModelViewer::load_rc_ass(QFile * qfile){
    makeCurrent();
    vertexAttribute_ass[tarnum++].Create();
    vector<float> empty;
    vector<float> colors;
    loadObj_rc(qfile, positions, normals, indices);
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
//    std::cout<<"load     : " << filename << std::endl;
//    std::cout<<"face num : " << indices.size()/3 << std::endl;
    vertexAttribute_ass[tarnum_ass-1].BufferData(positions, normals, empty, colors, indices);
    update();
    viewMgr->modelMgr_ass[tarnum_ass-1].Reset();
    viewMgr->modelMgr_ass[tarnum_ass-1].vertices = positions;
    viewMgr->modelMgr_ass[tarnum_ass-1].colors = colors;
    viewMgr->modelMgr_ass[tarnum_ass-1].vertices_ori = positions;
    viewMgr->modelMgr_ass[tarnum_ass-1].normals = normals;
    viewMgr->modelMgr_ass[tarnum_ass-1].indices = indices;
    viewMgr->modelMgr_ass[tarnum_ass-1].saveColors();

}

void ModelViewer::load(const char* filename){
    makeCurrent();
    vertexAttribute[tarnum++].Create();
    vector<float> empty;
    vector<float> colors;
    loadObj(filename, positions, normals, indices);
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
//    std::cout<<"load     : " << filename << std::endl;
//    std::cout<<"face num : " << indices.size()/3 << std::endl;
    vertexAttribute[tarnum-1].BufferData(positions, normals, empty, colors, indices);
    update();
    viewMgr->modelMgr[tarnum-1].Reset();
    viewMgr->modelMgr[tarnum-1].vertices = positions;
    viewMgr->modelMgr[tarnum-1].indices = indices;
    viewMgr->modelMgr[tarnum-1].vertices_ori = positions;
    viewMgr->modelMgr[tarnum-1].normals = normals;
    viewMgr->modelMgr[tarnum-1].regenNormals();
    viewMgr->modelMgr[tarnum-1].colors = colors;
    viewMgr->modelMgr[tarnum-1].saveColors();
}
void ModelViewer::reload(const char* filename,int tar){
    makeCurrent();
    if(tar>=tarnum)return;
    vertexAttribute[tar].Destroy();
    vertexAttribute[tar].Create();
    vector<float> empty;
    vector<float> colors;
    loadObj(filename, positions, normals, indices);
    viewMgr->modelMgr[tar].vertices_ori = positions;
    viewMgr->modelMgr[tar].applyModelMatrix_force();
    viewMgr->modelMgr[tar].indices = indices;
    viewMgr->modelMgr[tar].regenNormals();
    viewMgr->modelMgr[tar].setColors(0.5f,0.5f,0.5f);
    viewMgr->modelMgr[tar].saveColors();

    vertexAttribute[tar].BufferData(positions, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, indices);
    update();
}

void ModelViewer::copyObj(int tar){
    makeCurrent();
    vertexAttribute[tarnum++].Create();
    vector<float> vertices = viewMgr->modelMgr[tar].vertices_ori;
    vector<float> normals = viewMgr->modelMgr[tar].normals;
    vector<unsigned int> indices = viewMgr->modelMgr[tar].indices;
    vector<float> colors = viewMgr->modelMgr[tar].colors;
    vector<float> empty;
    vertexAttribute[tarnum-1].BufferData(vertices, normals, empty, colors, indices);
    viewMgr->modelMgr[tarnum-1] = viewMgr->modelMgr[tar];
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
            float planeh = (viewMgr->eyelookdis-viewMgr->clipdis)*2*tan(viewMgr->perspangle*viewMgr->zoom/2 * M_PI / 180.0 );
            float planew = planeh * winw / winh;
            Ray3 ray;
            ray.origin.setX( (event->x()-winw/2)        *planew/winw );
            ray.origin.setY( (-(event->y()-winh/2))     *planeh/winh );
            ray.origin.setZ( viewMgr->eyeLookPosition.z() + viewMgr->clipdis );
            ray.direction = ray.origin - viewMgr->eyePosition;
            QMatrix4x4 mat = viewMgr->GetViewTranslationMatrix().inverted() * viewMgr->GetViewRotationMatrix().inverted();
            ray.origin = mat * ray.origin;
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
                viewMgr->modelMgr[tarObj].paintSelecIdxs();
                reBuffer(tarObj);
                viewMgr->modelMgr[tarObj].loadColors();
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
