#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include "pch.h"
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
    std::vector<QVector3D> selecPoints;
    std::vector<int> selecIdxs;
    std::vector<int> detourIdxs;
    std::set<Edge> edges;
    std::vector<unsigned int> connectorFaceIdxs;
    /*basic management function*/
    void SetScale(float x, float y, float z);
    void normalize(float val);
    void genRandomColor();
    void paintCurvures();
    void SetScale(QVector3D xyz){applyed = false;scalex = xyz.x();scaley = xyz.y();scalez = xyz.z();}
    void SetRotation(QVector3D from, QVector3D to);
    void rotateX(float angle);
    void rotateY(float angle);
    void setViewRotation(QMatrix4x4 m);
    void translate(QVector3D movement);
    void setRotationAxis();
    void calNeighbor();
    void calCurvures(int rings);
    QVector3D GetScaleXYZ(){QVector3D xyz = {scalex,scaley,scalez};return xyz;}
    QMatrix4x4 GetScaleMatrix();
    QMatrix4x4 GetModelMatrix() {return translationMatrix * rotationMatrix * GetScaleMatrix();}
    void Reset();
    void ResetModel();
    void ResetView();
    void applyModelMatrix();
    void regenNormals();
    void fix();
    void setColors(float r,float g,float b);
    void saveColors(){colors_ori=colors;}
    void loadColors(){if(colors_ori.size()>0)colors=colors_ori;}
    void paintSelecIdxs();
    void paintDetour();
    void pushVertice_ori(QVector3D v);
    void pushColor(QVector3D c);
    void pushNormal(QVector3D n);
    void pushIndice(unsigned int a, unsigned int b, unsigned int c);
    QVector3D getVertice_ori(int idx);
    QVector3D getVertice(int idx);
    QVector3D getColor(int idx);
    QVector3D getNormal(int idx);
    std::vector<QVector3D> getSelecPointsByIdxs();
    std::vector<QVector3D> getSelecPointsByIdxs_ori();
    /*advanced calculation function*/
    std::vector<int> nRingNeighbor(int root, int rings);
    void fitAllSelecIdxs();
    void fitSelecIdxs(int tar);
    int calDetourByBfs();
    int calDetourByPlane(float radii);
    void cutByDetour();
    void cutByDetour_reverse();
    void fillByDetour();
    void regenByPlateIntersec();
    bool isCrossPlane(QVector3D v1, QVector3D v2, QVector3D s1, QVector3D s2, QVector3D s3);
    float pointDistanceToPlane(QVector3D v, QVector3D s1, QVector3D s2, QVector3D s3);
    float dotProduct(QVector3D a,QVector3D b);
    bool checkDetour();
    void gendetourPlane();
    void pullConnect();
    void pushConnect();
private:
    QMatrix4x4 scaleMatrix;
    QMatrix4x4 translationMatrix;
    QMatrix4x4 rotationMatrix;
    QMatrix4x4 viewRotationMatrix;
    QVector3D rotateXAxis;
    QVector3D rotateYAxis;
    float scalex, scaley, scalez;
    bool applyed = true;
    float minThre = (float)1.0e-5;
};
#endif // MODELMANAGER_H
