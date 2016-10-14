#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include "pch.h"
#include "looplist.h"
#define minThre ((float)(1.0e-4))

typedef std::vector<QVector3D> vecq3d;

class Edge{
public:
    Edge(unsigned int ia, unsigned int ib){
        this->ia = ia;
        this->ib = ib;
        if(this->ia>this->ib)swap(this->ia,this->ib);
    }
    bool operator < (const Edge& edge) const
    {
        if(this->ia == edge.ia){
            return this->ib < edge.ib;
        }
        return this->ia < edge.ia;
    }
    bool operator == (const Edge& edge) const
    {
        return (this->ia == edge.ia && this->ib == edge.ib);
    }
    bool operator > (const Edge& edge) const
    {
        if(this->ia == edge.ia){
            return this->ib > edge.ib;
        }
        return this->ia > edge.ia;
    }
    unsigned int mid;
private:
    void swap(unsigned int &a, unsigned int &b){
        unsigned int t;
        t=a;
        a=b;
        b=t;
    }
    unsigned int ia, ib;
};

class Plane{
public:
    QVector3D normal;
    QVector3D center;
    float radii;//option
    bool exist=false;
    Plane(){
        exist=false;
    }
    Plane(QVector3D normal, QVector3D center){
        this->normal=normal;
        this->center=center;
        exist=true;
    }
    Plane(QVector3D a, QVector3D b, QVector3D c){
        QVector3D center = (a+b+c)/3;
        QVector3D normal = QVector3D::crossProduct(a-center,b-center).normalized();
        this->normal=normal;
        this->center=center;
        exist=true;
    }
    bool isExist(){
        return exist;
    }
    void setRadii(float radii){
        this->radii = radii;
    }
    float distanceToPoint(QVector3D v){
        QVector3D vc = center-v;
        return -(vc.x()*normal.x()+vc.y()*normal.y()+vc.z()*normal.z());
    }
    bool isCrossBy(QVector3D v1, QVector3D v2){
        QVector3D c = center;
        QVector3D n = normal;
        QVector3D v1c = (c-v1);
        QVector3D v2c = (c-v2);
        float d1 = v1c.x()*n.x()+v1c.y()*n.y()+v1c.z()*n.z();
        float d2 = v2c.x()*n.x()+v2c.y()*n.y()+v2c.z()*n.z();

        if(std::fabs(d1) < minThre){
            return false;
        }else if(std::fabs(d2) < minThre){
            return false;
        }else if(d1*d2<0){
            return true;
        }
        return false;
    }
};

class ModelManager
{
public:
    ModelManager();
    /**valiable**/
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> colors;
    std::vector<float> colors_ori;
    std::vector<float> vertices_ori;
    std::vector<float> curvures;
    std::vector<std::vector<unsigned int>> neighbor;
    std::vector<unsigned int> indices;
    vecq3d selecPoints;
    std::vector<int> selecIdxs;
    std::vector<int> detourIdxs;
    std::vector<int> detourIdxs_all;//include vertices inside
    std::set<Edge> edges;
    std::vector<unsigned int> connectorFaceIdxs;
    std::vector<unsigned int> connectorFaceIdxs_sup;
    bool connectorReady = false;
    bool connectorFaceReady = false;
    QVector3D connectorNormal_ori;
    QVector3D connectorCenter_ori;
    float connectorRadii_ori;
    float connectorRadii2_ori;
    Plane cuttingPlane;
    bool connectReverse = false;
    int detourSPIdx;//start point
    int detourCPIdx;//center point
    /*basic management function*/
    void SetScale(float x, float y, float z);
    void normalize(float val);
    void genRandomColor();
    void paintCurvures();
    void SetScale(QVector3D xyz){applyed = false;scalex = xyz.x();scaley = xyz.y();scalez = xyz.z();}
    void SetRotation(QVector3D from, QVector3D to);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateTo(QVector3D vec);
    void setViewRotation(QMatrix4x4 m);
    void translate(QVector3D movement);
    void translate_pure(QVector3D movement);
    void setRotationAxis();
    void calNeighbor();
    void calCurvures(int rings);
    QVector3D GetScaleXYZ(){QVector3D xyz = {scalex,scaley,scalez};return xyz;}
    QMatrix4x4 GetScaleMatrix();
    QMatrix4x4 GetModelMatrix() {return translationMatrix * rotationMatrix * GetScaleMatrix();}
    void Reset();
    void ResetModel();
    void ResetView();
    void applyModelMatrix_force();
    void applyModelMatrix();
    void regenNormals();
    void scaleDepend_ori(QVector3D c, float valx, float valy, float valz);
    void fix();
    void setColors(float r,float g,float b);
    void saveColors(){colors_ori=colors;}
    void loadColors(){if(colors_ori.size()>0)colors=colors_ori;}
    void paintSelecIdxs();
    void paintDetour();
    unsigned int pushVertice_ori(QVector3D v);
    void putVertice_ori(unsigned int idx,QVector3D v);
    void putColor(unsigned int idx,QVector3D c);
    void pushColor(QVector3D c);
    void pushNormal(QVector3D n);
    void pushIndice(unsigned int a, unsigned int b, unsigned int c);
    vecq3d getIndicesVertice_ori(int idx);
    QVector3D getCenter_ori();
    QVector3D getMassCenter_ori();
    QVector3D getVertice_ori(int idx);
    QVector3D getVertice(int idx);
    QVector3D getColor(int idx);
    QVector3D getNormal_ori(int idx);//ori
    void refresh_with_normalize(float scale);
    void refresh();
    void clearSupportData();
    void ResetConnector();
    void ResetMainData();
    vecq3d getSelecPointsByIdxs();
    vecq3d getSelecPointsByIdxs_ori();
    void clearConnector();
    /*advanced calculation function*/
    std::vector<int> nRingNeighbor(int root, int rings);
    void fitAllSelecIdxs();
    void fitSelecIdxs(int tar);
    int calDetourByPlane();
    int calDetourByPlane(QVector3D c,QVector3D n);
    void cutByDetour(int state);
    void fillByDetour();
    void regenByPlateIntersec();
    bool checkDetour();
    void pullConnect(float val);
    void pushConnect(float val);
    void circleOnPlane(QVector3D c, QVector3D n, float radii, int div, int cmp);
    void circleOnPlane(QVector3D c, QVector3D n, float radii, int div);
    void linkContour(Plane mainPlane, std::vector<int> contourIdx);
    void produceFace();
    std::vector<int> bfs(std::vector<bool> &visited, int rootIdx);
    /*data support*/
    QVector3D detourNormal();
    QVector3D selecPointsNormal();
    QVector3D detourNormal_ori();
    QVector3D selecPointsNormal_ori();
    QVector3D selecPointsCenter();
    QVector3D selecPointsCenter_ori();
    QVector3D detourCenter();
    QVector3D detourCenter_ori();
    QVector3D detourNormal_ori(std::vector<int> detourIdxs_this);
    QVector3D detourCenter_ori(std::vector<int> detourIdxs_this);
    void moveVertice_ori(int idx, float x, float y, float z);
    void reverseDetours();
    void sortDetour();
    bool isCrossPlane(QVector3D v1, QVector3D v2, QVector3D s1, QVector3D s2, QVector3D s3);
    float pointDistanceToPlane(QVector3D v, QVector3D s1, QVector3D s2, QVector3D s3);
    float dotProduct(QVector3D a,QVector3D b);
//    /*advanced calculation function, unused code backup*/
//    int calDetourByBfs();
//    void gendetourPlane(QVector3D c);
//    void gendetourPlane();
//    void gendetourPlane_simple(QVector3D c);
//    void gendetourPlane_patch();
//    void gendetourPlane_convex();
//    void gendetourPlane_convex(std::vector<int> detourIdxs_this);
//    void cutByDetour_reverse();
private:
    QMatrix4x4 scaleMatrix;
    QMatrix4x4 translationMatrix;
    QMatrix4x4 rotationMatrix;
    QMatrix4x4 viewRotationMatrix;
    QVector3D rotateXAxis;
    QVector3D rotateYAxis;
    float scalex, scaley, scalez;
    bool applyed = true;
    std::vector<std::vector<int>> detourIdxsQueue;
    int detourIdxsQueue_flag = 0;
    std::vector<std::vector<int>> detourIdxsList;
    vecq3d detourCenterList;
};
#endif // MODELMANAGER_H
