#include "modelmanager.h"
#include "pch.h"

void print(std::string str){
    std::cout << str << std::endl;
}

bool ModelManager::checkDetour(){
    std::vector<std::set<unsigned int>> neighborset;
    for(int i=0;i<(int)vertices_ori.size()/3;i++){
        std::set<unsigned int> pushor;
        neighborset.push_back(pushor);
    }
    int vn = (int)vertices_ori.size()/3;
    bool * exsit;exsit = (bool*)malloc(sizeof(bool)*vn);memset(exsit,false,vn);
    for(int i=0;i<(int)detourIdxs.size();i++){
        exsit[detourIdxs[i]]=true;
    }
    //std::vector<QVector3D> s = getSelecPointsByIdxs();
    for(int i=0;i<(int)indices.size();i+=3){
        if(exsit[indices[i]] && exsit[indices[i+1]]){
            neighborset[indices[i]].insert(indices[i+1]);neighborset[indices[i+1]].insert(indices[i]);
        }
        if(exsit[indices[i+2]] && exsit[indices[i+1]]){
            neighborset[indices[i+1]].insert(indices[i+2]);neighborset[indices[i+2]].insert(indices[i+1]);
        }
        if(exsit[indices[i+2]] && exsit[indices[i]]){
            neighborset[indices[i]].insert(indices[i+2]);neighborset[indices[i+2]].insert(indices[i]);
        }
    }
    int begin = detourSPIdx;
    int cur = begin;
    detourIdxs.clear();
    while(true){
        if(neighborset[cur].empty())break;
        int tar = *neighborset[cur].begin();
        neighborset[cur].erase(tar);
        neighborset[tar].erase(cur);
        detourIdxs.push_back(cur);
        cur = tar;
        if(cur == begin)break;
    }
    if(cur == begin)return true;
    return false;
}

void ModelManager::sortDetour(){
    QVector3D c = cuttingPlane.center;
    QVector3D nv(0,0,0);
    for(int i=0;i<indices.size();i+=3){
        for(int j=0;j<3;j++){
            int vai = indices[i+j];
            int vbi = indices[i+(j+1)%3];
            QVector3D va = getVertice(vai);
            QVector3D vb = getVertice(vbi);
            if(std::fabs(cuttingPlane.distanceToPoint(va))<minThre && std::fabs(cuttingPlane.distanceToPoint(vb))<minThre){
                nv+=QVector3D::crossProduct(va-c,vb-c);
            }
        }
    }
    nv.normalize();
    QVector3D nd = detourNormal();
    if(dotProduct(nv,nd) > 0){
        reverseDetours();
    }
}

void ModelManager::cutByDetour(int state){
    if(detourIdxs.size()==0)return;
    if(neighbor.size()==0)calNeighbor();
    int vn = (int)vertices_ori.size()/3;
    std::vector<int> queue;
    bool * visited;
    visited = (bool*)malloc(sizeof(bool)*vn);
    memset(visited,false,vn);
    for(int i=0;i<(int)detourIdxs.size();i++){
        visited[detourIdxs[i]]=true;
    }
    int rootIdx = -1;
    QVector3D c = detourCenter();
    QVector3D n = detourNormal();
    for(int i=0;i<(int)vertices.size()/3;i++){
        QVector3D v = getVertice(i);
        if(state>0){
            if(!visited[i] && dotProduct(v-c,n) < 0){
                rootIdx=i;
                break;
            }
        }else if (state<0){
            if(!visited[i] && dotProduct(v-c,n) > 0){
                rootIdx=i;
                break;
            }
        }
    }
    queue.clear();
    queue.push_back(rootIdx);
    visited[rootIdx]=true;
    int head = 0;
    while(head<(int)queue.size()){
        int qs = (int)queue.size();
        for(int k=head;k<qs;k++){
            int a = queue[k];
            for(int l=0;l<(int)neighbor[a].size();l++){
                int b = neighbor[a][l];
                if(!visited[b]){
                    visited[b] = true;
                    queue.push_back(b);
                }
            }
        }
        head = qs;
    }
    std::vector<unsigned int> indices_new;
    for(int i=0;i<(int)indices.size();i+=3){
        if(visited[indices[i]]&&visited[indices[i+1]]&&visited[indices[i+2]]){
            indices_new.push_back(indices[i]);
            indices_new.push_back(indices[i+1]);
            indices_new.push_back(indices[i+2]);
        }
    }
    indices = indices_new;
    fix();
    neighbor.clear();
    curvures.clear();
    sortDetour();
    //gendetourPlane();//gendetourPlane_convex();//gendetourPlane_patch();//fillByDetour();
    //regenNormals();
    connectorNormal_ori = detourNormal_ori();
    connectorCenter_ori = detourCenter_ori();
    connectorRadii_ori=FLT_MAX;
    for(int i=0;i<detourIdxs.size();i++){
        connectorRadii_ori = std::min(connectorRadii_ori,(getVertice_ori(detourIdxs[i])-connectorCenter_ori).length());
    }

    setColors(0.5f,0.5f,0.5f);
    applyed=false;applyModelMatrix();
}

void ModelManager::calCurvures(int rings){
    applyModelMatrix();
    std::vector<QVector3D>verts;
    std::vector<QVector3D>norms;
    for(int i=0;i<(int)vertices.size();i+=3){
        verts.push_back(QVector3D(vertices[i],vertices[i+1],vertices[i+2]));
    }
    for(int i=0;i<(int)normals.size();i+=3){
        norms.push_back(QVector3D(normals[i],normals[i+1],normals[i+2]).normalized());
    }
    curvures.clear();
    int vn = (int)verts.size();
    for(int i=0;i<vn;i++){
        std::vector<int> queue;
        bool * visited;
        visited = (bool*)malloc(sizeof(bool)*vn);
        queue.clear();
        memset(visited,false,vn);
        queue.push_back(i);
        visited[i]=true;
        float cal = 0;
        int num = 0;
        int head = 0;
        for(int j=0;j<rings;j++){
            int qs = (int)queue.size();
            for(int k=head;k<qs;k++){
                int a = queue[k];
                for(int l=0;l<(int)neighbor[a].size();l++){
                    int b = neighbor[a][l];
                    if(!visited[b]){
                        cal += dotProduct(norms[a]-norms[b],verts[a]-verts[b])/(verts[a]-verts[b]).lengthSquared();
                        num++;
                        visited[b] = true;
                        queue.push_back(b);
                    }
                }
            }
            head = qs;
        }
        curvures.push_back(cal/num);
    }
}

void ModelManager::calNeighbor(){

    //fix();
    std::vector<std::set<unsigned int>> neighborset;
    for(int i=0;i<(int)vertices_ori.size()/3;i++){
        std::set<unsigned int> pushor;
        neighborset.push_back(pushor);
    }
    for (int i = 0; i<(int) indices.size(); i+=3)
    {
        unsigned int ia = indices[i];
        unsigned int ib = indices[i+1];
        unsigned int ic = indices[i+2];
        neighborset[ia].insert(ib);neighborset[ia].insert(ic);
        neighborset[ib].insert(ia);neighborset[ib].insert(ic);
        neighborset[ic].insert(ia);neighborset[ic].insert(ib);

    }
    neighbor.clear();
    for (int i = 0; i<(int) neighborset.size(); i++){
        std::vector<unsigned int> pushor;
        //for (std::set<int>::iterator itrt = neighborset[i].begin(); itrt != neighborset[i].end(); itrt++){
        for(auto itrt : neighborset[i]){
            pushor.push_back(itrt);
        }
        neighbor.push_back(pushor);
    }
}

std::vector<int> ModelManager::nRingNeighbor(int rootIdx, int rings){
    if(neighbor.size()==0)calNeighbor();
    int vn = (int)vertices_ori.size()/3;
    std::vector<int> queue;
    bool * visited;
    visited = (bool*)malloc(sizeof(bool)*vn);
    queue.clear();
    memset(visited,false,vn);
    queue.push_back(rootIdx);
    visited[rootIdx]=true;
    int head = 0;
    for(int j=0;j<rings;j++){
        int qs = (int)queue.size();
        for(int k=head;k<qs;k++){
            int a = queue[k];
            for(int l=0;l<(int)neighbor[a].size();l++){
                int b = neighbor[a][l];
                if(!visited[b]){
                    visited[b] = true;
                    queue.push_back(b);
                }
            }
        }
        head = qs;
    }
    return queue;
}

int ModelManager::calDetourByPlane(float radii){
    if(selecIdxs.size()<3)return 0;
    std::vector<QVector3D> s;
    s = getSelecPointsByIdxs_ori();
    cuttingPlane = Plane(s[0],s[1],s[2]);
    regenByPlateIntersec();//contain "detourIdxs.clear();"
    calNeighbor();
    QVector3D c  = cuttingPlane.center;
    bool * visited;visited = (bool*)malloc(sizeof(bool)*vertices.size());memset(visited,false,vertices.size());
    for(int i=0;i<(int)vertices.size()/3;i++){
        if(std::fabs(cuttingPlane.distanceToPoint(getVertice_ori(i)))<minThre){//pointDistanceToPlane(getVertice(i),s[0],s[1],s[2])
                visited[i]=true;
                detourIdxs.push_back(i);
            //}
        }
    }
    float mindis = FLT_MAX;
    for(int i=0;i<detourIdxs.size();i++){
        QVector3D v = getVertice_ori(detourIdxs[i]);
        if(dotProduct(v-c,getNormal_ori(detourIdxs[i]))>0){
            float vcl = std::fabs((v-c).length());
            if(mindis > vcl){
                mindis = vcl;
                detourSPIdx = detourIdxs[i];
            }
        }
    }

    if(checkDetour())return 1;
    else {
        detourIdxs.clear();
        return 0;
    }
}

void ModelManager::regenByPlateIntersec(){//must use model vertices, check all vertices;
    if(!cuttingPlane.isExist())return;
    edges.clear();
    int ids = (int)indices.size();
    for(int j=0;j<ids;j+=3){
        bool interseced[3];memset(interseced,false,sizeof(interseced));
        QVector3D v[5];
        v[0] = getVertice_ori(indices[j]);
        v[1] = getVertice_ori(indices[j+1]);
        v[2] = getVertice_ori(indices[j+2]);
        QVector3D cl[2];
        QVector3D nm[2];
        int flag=3;
        for(int i=0;i<3;i++){
            float d1 = std::fabs(cuttingPlane.distanceToPoint(v[i]));      //pointDistanceToPlane(v[i],s[0],s[1],s[2])
            float d2 = std::fabs(cuttingPlane.distanceToPoint(v[(i+1)%3]));//pointDistanceToPlane(v[(i+1)%3],s[0],s[1],s[2])
            if(cuttingPlane.isCrossBy(v[i], v[(i+1)%3])){//isCrossPlane(v[i], v[(i+1)%3], s[0], s[1], s[2])
                v[flag]=(v[(i+1)%3]-v[i])*d1/(d1+d2) + v[i];
                cl[flag-3] = (getColor(indices[j+i])+getColor(indices[j+(i+1)%3]))/2;
                nm[flag-3] = (getNormal_ori(indices[j+i])+getNormal_ori(indices[j+(i+1)%3]))/2;
                interseced[i]=true;
                Edge edge(indices[j+i], indices[j+(i+1)%3]);
                if(edges.count(edge)==0){
                    edge.mid=vertices_ori.size()/3;
                    edges.insert(edge);
                    vertices_ori.push_back(v[flag].x());vertices_ori.push_back(v[flag].y());vertices_ori.push_back(v[flag].z());
                    colors.push_back(cl[flag-3].x());colors.push_back(cl[flag-3].y());colors.push_back(cl[flag-3].z());
                    normals.push_back(nm[flag-3].x());normals.push_back(nm[flag-3].y());normals.push_back(nm[flag-3].z());
                }
                flag++;
            }
        }
        if(flag==5){
            int idx[5];
            idx[0]=indices[j];idx[1]=indices[j+1];idx[2]=indices[j+2];
            if(interseced[0]&&interseced[1]){
                idx[3]=edges.find(Edge(indices[j],indices[j+1]))->mid;
                idx[4]=edges.find(Edge(indices[j+1],indices[j+2]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
                indices.push_back(idx[2]);indices.push_back(idx[3]);indices.push_back(idx[4]);
                indices.push_back(idx[4]);indices.push_back(idx[3]);indices.push_back(idx[1]);
            }else if(interseced[1]&&interseced[2]){
                idx[3]=edges.find(Edge(indices[j+1],indices[j+2]))->mid;
                idx[4]=edges.find(Edge(indices[j+2],indices[j]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                indices.push_back(idx[3]);indices.push_back(idx[4]);indices.push_back(idx[0]);
                indices.push_back(idx[2]);indices.push_back(idx[4]);indices.push_back(idx[3]);
            }else if(interseced[2]&&interseced[0]){
                idx[3]=edges.find(Edge(indices[j],indices[j+1]))->mid;
                idx[4]=edges.find(Edge(indices[j+2],indices[j]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[4]);
                indices.push_back(idx[4]);indices.push_back(idx[3]);indices.push_back(idx[1]);
                indices.push_back(idx[1]);indices.push_back(idx[2]);indices.push_back(idx[4]);
            }
            indices[j]=indices[j+1]=indices[j+2]=0xffffffff;
        }else if(flag==4){
            int idx[5];
            idx[0]=indices[j];idx[1]=indices[j+1];idx[2]=indices[j+2];
            if(interseced[0]){
                idx[3]=edges.find(Edge(indices[j],indices[j+1]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
                indices.push_back(idx[3]);indices.push_back(idx[1]);indices.push_back(idx[2]);
            }else if(interseced[1]){
                idx[3]=edges.find(Edge(indices[j+1],indices[j+2]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
            }else if(interseced[2]){
                idx[3]=edges.find(Edge(indices[j],indices[j+2]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                indices.push_back(idx[3]);indices.push_back(idx[1]);indices.push_back(idx[2]);
            }
            indices[j]=indices[j+1]=indices[j+2]=0xffffffff;
        }
    }

    std::vector<unsigned int> indices_new;
    for(int j=0;j<(int)indices.size();j++){
        if(indices[j]!=0xffffffff){
            indices_new.push_back(indices[j]);
        }
    }
    indices.clear();
    for(int j=0;j<(int)indices_new.size();j++){
        indices.push_back(indices_new[j]);
    }
    neighbor.clear();
    detourIdxs.clear();
    applyed=false;applyModelMatrix();
    saveColors();
}

void ModelManager::pullConnect(){
    if(connectorFaceIdxs.size()==0)return;
    QVector3D n = connectorNormal_ori;
    for(int i=0;i<(int)connectorFaceIdxs.size();i++){
        QVector3D v = getVertice_ori(connectorFaceIdxs[i]);
        v = v + n * 5.0f / scalex;
        vertices_ori[connectorFaceIdxs[i]*3]  = v.x();
        vertices_ori[connectorFaceIdxs[i]*3+1]= v.y();
        vertices_ori[connectorFaceIdxs[i]*3+2]= v.z();
    }
    regenNormals();
    applyed=false;applyModelMatrix();
}

void ModelManager::pushConnect(){
    if(connectorFaceIdxs.size()==0)return;
    if(connectorFaceIdxs.size()==0)return;
    QVector3D n = connectorNormal_ori;
    for(int i=0;i<(int)connectorFaceIdxs.size();i++){
        QVector3D v = getVertice_ori(connectorFaceIdxs[i]);
        v = v - n * 5.0f / scalex;
        vertices_ori[connectorFaceIdxs[i]*3]  = v.x();
        vertices_ori[connectorFaceIdxs[i]*3+1]= v.y();
        vertices_ori[connectorFaceIdxs[i]*3+2]= v.z();
    }
    regenNormals();
    applyed=false;applyModelMatrix();
}


