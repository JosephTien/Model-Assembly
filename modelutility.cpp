#include "modelmanager.h"
#include "pch.h"
float absf(float n){
    return n>=0?n:-n;
}
void print(std::string str){
    std::cout << str << std::endl;
}

int ModelManager::calDetourByBfs(){ //bad selection? //not yet debug
    if(selecIdxs.size()<3)return 1;
    applyed=false;
    applyModelMatrix();
    if(neighbor.size()==0)calNeighbor();
    QVector3D pa,pb,po,pn;
    int pai,pbi,poi;
    detourIdxs.clear();

    for(int i=0;i<3;i++){
        pai = selecIdxs[i];pbi = selecIdxs[(i+1)%3];poi = selecIdxs[(i+2)%3];
        pa=QVector3D(vertices[pai*3],vertices[pai*3+1],vertices[pai*3+2]);
        pb=QVector3D(vertices[pbi*3],vertices[pbi*3+1],vertices[pbi*3+2]);
        po=QVector3D(vertices[poi*3],vertices[poi*3+1],vertices[poi*3+2]);
        pn = QVector3D::crossProduct(pa-po,pb-po).normalized();
        std::vector<int> queue_idx;
        bool * visited;
        visited = (bool*)malloc(sizeof(bool)*vertices.size());
        memset(visited,false,vertices.size());
        float * sum;
        sum = (float*)malloc(sizeof(bool)*vertices.size());
        memset(sum,FLT_MAX,vertices.size());
        int * from;
        from = (int*)malloc(sizeof(bool)*vertices.size());
        memset(from,-1,vertices.size());
        int * belong;
        belong = (int*)malloc(sizeof(bool)*vertices.size());
        memset(belong,-1,vertices.size());
        queue_idx.push_back(pai);
        sum[pai]=0;
        belong[pai]=0;
        int flag=0;
        bool sign=false;
        while(!sign){
            int qs = (int)queue_idx.size();
            for(int j=flag;j<qs;j++){
                int curparidx = queue_idx[j];
                if(queue_idx[j]==pbi){
                    std::vector<unsigned int> route;
                    for(int k=from[queue_idx[j]];k>=0;k=from[k]){
                        route.push_back(k);
                    }
                    for(int k=(int)route.size()-1;k>=0;k--){
                        detourIdxs.push_back(route[k]);
                    }
                    sign=true;
                    break;
                }
                for(int k=0;k<(int)neighbor[queue_idx[j]].size();k++){
                    int curneiidx = neighbor[queue_idx[j]][k];
                    if(curneiidx==-1)continue;
                    if(visited[curneiidx])continue;
                    QVector3D curneiv = getVertice(curneiidx);
                    float curneival = sum[curparidx] + pointDistanceToPlane(curneiv,pa,pb,po);
                    if(curneival < sum[curneiidx]){
                        if(belong[curneiidx]>=0)queue_idx[belong[curneiidx]]=-1;
                        belong[curneiidx] = (int)queue_idx.size();
                        sum[curneiidx] = curneival;
                        from[curneiidx] = curparidx;
                        queue_idx.push_back(curneiidx);
                    }
                }
            }
            for(int j=flag;j<qs;j++){
                visited[queue_idx[j]] = true;
            }
            flag=qs;
        }

        /*
        detourIdxs.push_back(pai);
        int curIdx = pai;
        int cnt = 0;
        while(curIdx!=pbi){
            //std::cout << i << " : " << curIdx << std::endl;
            if(cnt++>vertices.size()/100){//fetal problem
                std::cout << " detour failed " << std::endl;
                detourIdxs.clear();
                return -1;
            }
            QVector3D pc = QVector3D(vertices[curIdx*3],vertices[curIdx*3+1],vertices[curIdx*3+2]);
            float minDis = 9999.0f;int minIdx = 0;
            for(int j=0;j<(int)neighbor[curIdx].size();j++){
                QVector3D pt = QVector3D(vertices[neighbor[curIdx][j]*3],vertices[neighbor[curIdx][j]*3+1],vertices[neighbor[curIdx][j]*3+2]);
                if(dotProduct(QVector3D::crossProduct(pc-po,pt-po), pn) < 0)continue;//problem
                float dtp = pointDistanceToPlane(pt,pa,pb,po);
                dtp = dtp>=0?dtp:-dtp;
                if(dtp<minDis){
                    minDis=dtp;
                    minIdx=j;
                }
            }
            detourIdxs.push_back(neighbor[curIdx][minIdx]);
            visited[neighbor[curIdx][minIdx]]=true;
            curIdx = neighbor[curIdx][minIdx];
        }
        */
    }
    return 0;
}

void ModelManager::fillByDetour(){ //todo 外心 距離原心 繞圈檢查
    QVector3D p1 = getVertice_ori(selecIdxs[0]);
    QVector3D p2 = getVertice_ori(selecIdxs[1]);
    QVector3D p3 = getVertice_ori(selecIdxs[2]);
    QVector3D c = (p1+p2+p3)/3;
    QVector3D cl= (getColor(selecIdxs[0])+getColor(selecIdxs[1])+getColor(selecIdxs[2]))/3;
    QVector3D nm = QVector3D::crossProduct(p2-p1, p3-p1).normalized();
    int vn = (int)vertices_ori.size()/3;
    bool * exsit;exsit = (bool*)malloc(sizeof(bool)*vn);memset(exsit,false,vn);
    for(int i=0;i<(int)detourIdxs.size();i++){
        exsit[detourIdxs[i]] = true;
    }
    int ids = (int)indices.size();
    for(int i=0;i<ids;i+=3){
        for(int j=0;j<3;j++){
            int vai = indices[i+j];
            int vbi = indices[i+(j+1)%3];
            if(exsit[vai] && exsit[vbi]){
                indices.push_back(vbi);
                indices.push_back(vai);
                indices.push_back(vn);
            }
            nm = QVector3D::crossProduct(getVertice_ori(vbi)-c,getVertice_ori(vai)-c).normalized();
            QVector3D van = getNormal(vai).normalized();
            van = (van + nm)/2;
            //normals[vai*3] = van.x(); normals[vai*3+1] = van.y(); normals[vai*3+2] = van.z();
        }
    }
    vertices_ori.push_back(c.x());vertices_ori.push_back(c.y());vertices_ori.push_back(c.z());
    normals.push_back(nm.x());normals.push_back(nm.y());normals.push_back(nm.z());
    colors.push_back(cl.x());colors.push_back(cl.y());colors.push_back(cl.z());
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
    std::vector<QVector3D> s = getSelecPointsByIdxs();
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
    int cur = selecIdxs[0];
    detourIdxs.clear();
    while(true){
        if(neighborset[cur].empty())break;
        int tar = *neighborset[cur].begin();
        neighborset[cur].erase(tar);
        neighborset[tar].erase(cur);
        detourIdxs.push_back(cur);
        cur = tar;
        if(cur == selecIdxs[0])break;
    }
    if(cur == selecIdxs[0])return true;
    return false;
}

void ModelManager::sortDetour(){
    std::vector<QVector3D> s = getSelecPointsByIdxs();
    QVector3D c = detourCenter();
    QVector3D nv(0,0,0);
    for(int i=0;i<indices.size();i+=3){
        for(int j=0;j<3;j++){
            int vai = indices[i+j];
            int vbi = indices[i+(j+1)%3];
            QVector3D va = getVertice(vai);
            QVector3D vb = getVertice(vbi);
            if(abs(pointDistanceToPlane(va,s[0],s[1],s[2]))<minThre && abs(pointDistanceToPlane(vb,s[0],s[1],s[2]))<minThre){
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

void ModelManager::cutByDetour_reverse(){
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
        if(!visited[i] && dotProduct(v-c,n) > 0){
            rootIdx=i;
            break;
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
    //fillByDetour();
    gendetourPlane();
    regenNormals();
    setColors(0.5f,0.5f,0.5f);
    applyed=false;applyModelMatrix();
}

void ModelManager::cutByDetour(){
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
        if(!visited[i] && dotProduct(v-c,n) < 0){
            rootIdx=i;
            break;
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
    //fillByDetour();
    gendetourPlane();
    regenNormals();
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
        /*
        std::cout << i << " / " << indices.size()<< std::endl;
        std::cout << "      " << ia << " " << ib << " " << ic << std::endl;
        std::cout << "            " << vertices_ori.size()/3 << std::endl;
        std::cout << "                  " << neighborset.size() << std::endl;
        */
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

//    for (int i = 0; i<(int) indices.size(); i+=3)
//    {
//        int ia = (int)indices[i];
//        int ib = (int)indices[i+1];
//        int ic = (int)indices[i+2];
//        neighbor[ia].push_back(ib);neighbor[ia].push_back(ic);
//        neighbor[ib].push_back(ia);neighbor[ib].push_back(ic);
//        neighbor[ic].push_back(ia);neighbor[ic].push_back(ib);
//    }

//    for (int i = 0; i<(int) neighbor.size(); i++)
//    {
//        /* sort
//        for(int j=0;j<(int)neighbor[i].size();i++){
//            unsigned int t = neighbor[i][j];
//            for(int k=j-1;k>=0;k--){
//                if(neighbor[i][k]>t){
//                   neighbor[i][k+1] = neighbor[i][k];
//                }else{
//                   neighbor[i][k+1] = t;
//                   break;
//                }
//            }

//        }
//        */
//        for(int j=0;j<(int)neighbor[i].size();j++){
//            for(int k=j+1;k<neighbor[i].size();k++){
//                if(neighbor[i][j] == neighbor[i][k]){
//                    unsigned int t = neighbor[i][k];
//                    neighbor[i][k] = neighbor[i][neighbor[i].size()-1];
//                    neighbor[i][neighbor[i].size()-1] = t;
//                    neighbor[i].pop_back();
//                    break;
//                }
//            }
//        }
//    }
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
    regenByPlateIntersec();//detourIdxs.clear();
    calNeighbor();
    std::vector<QVector3D> s;
    s = getSelecPointsByIdxs();
    QVector3D c  = (s[0]+s[1]+s[2])/3;
    bool * visited;visited = (bool*)malloc(sizeof(bool)*vertices.size());memset(visited,false,vertices.size());
    for(int i=0;i<(int)vertices.size()/3;i++){
        if(absf(pointDistanceToPlane(getVertice(i),s[0],s[1],s[2]))<minThre){
            if((getVertice(i)-c).length()<radii){
                visited[i]=true;
                detourIdxs.push_back(i);
            }
        }
    }

    if(checkDetour())return 1;
    else {
        detourIdxs.clear();
        return 0;
    }

    /*
    bool * visited;visited = (bool*)malloc(sizeof(bool)*vertices.size());memset(visited,false,vertices.size());
    int curIdx = selecIdxs[0];
    int cnt=0;
    while(true){
        std::cout << cnt++ << std::endl;
        boolean valid=false;
        for(int i=0;i<(int)neighbor[curIdx].size();i++){
            int tarIdx = neighbor[curIdx][i];
            float val = absf(pointDistanceToPlane(getVertice(curIdx),s1,s2,s3));
            if(val>minThre)continue;
            if(visited[tarIdx])continue;

            std::cout << val << std::endl;
            valid=true;
            curIdx = tarIdx;
            detourIdxs.push_back(curIdx);
            visited[curIdx]=true;
            break;
        }
        if(!valid)break;
    }

    if(curIdx==selecIdxs[0]){
        std::cout << "well detour" << std::endl;
        return 1;
    }else{
        std::cout << "detour failed" << std::endl;
        return 0;
    }
    */

}

float ModelManager::dotProduct(QVector3D a,QVector3D b){
    return a.x()*b.x()+a.y()*b.y()+a.z()*b.z();
}
float ModelManager::pointDistanceToPlane(QVector3D v,QVector3D s1, QVector3D s2, QVector3D s3){
    QVector3D c((s1+s2+s3)/3);
    QVector3D n = QVector3D::crossProduct(s2-s1, s3-s1).normalized();
    QVector3D vc = c-v;
    return -(vc.x()*n.x()+vc.y()*n.y()+vc.z()*n.z());
}

bool ModelManager::isCrossPlane(QVector3D v1, QVector3D v2, QVector3D s1, QVector3D s2, QVector3D s3){
    QVector3D c((s1+s2+s3)/3);
    QVector3D n = QVector3D::crossProduct(s2-s1, s3-s1).normalized();
    QVector3D v1c = (c-v1);
    QVector3D v2c = (c-v2);
    float d1 = v1c.x()*n.x()+v1c.y()*n.y()+v1c.z()*n.z();
    float d2 = v2c.x()*n.x()+v2c.y()*n.y()+v2c.z()*n.z();
    if(absf(d1)<minThre || absf(d2)<minThre){
        return false;
    }else if(d1*d2<0){
        return true;
    }
    return false;
}
void ModelManager::gendetourPlane(){//assume sorted
    std::vector<QVector3D> detourPoints;
    std::vector<unsigned int> detourIdxs_this;
    std::vector<unsigned int> detourIdxs_next;
    QVector3D c(0.0f,0.0f,0.0f);
    for(int i=0;i<(int)detourIdxs.size();i++){
        detourIdxs_this.push_back(detourIdxs[i]);
        QVector3D v = getVertice_ori(detourIdxs[i]);
        detourPoints.push_back(v);
        c = c + v;
    }
    c = c / (float)(detourIdxs.size());
    QVector3D n = detourNormal_ori();
    float d=0,r=0;
    for(int i=1;i<(int)detourPoints.size();i++){
        d+=(detourPoints[i-1]-detourPoints[i]).length();
        r+=(c-detourPoints[i]).length();
    }
    int rings=((int)(r/d))-1;
    connectorFaceIdxs.clear();
    float interval = 0.5f;
    for(int i=0;i<rings;i++){
        int vs = vertices_ori.size()/3;
        detourIdxs_next.clear();
        for(int j=0;j<(int)detourPoints.size();j++){
            QVector3D arr = (c-detourPoints[j]);
            QVector3D v = detourPoints[j] + (arr *((i+1.0f)/(rings+1.0f)));
            pushVertice_ori(v);
            pushNormal(n);
            pushColor(QVector3D(0.5f,0.5f,0.5f));
            detourIdxs_next.push_back(vs+j);
            if(i>(int)(rings*interval)){
                connectorFaceIdxs.push_back(vs+j);
            }
        }
        int ds = (int)detourIdxs_this.size();
        for(int j=0;j<ds;j++){
            pushIndice(detourIdxs_this[j],detourIdxs_this[(j+1)%ds],detourIdxs_next[j]);
            pushIndice(detourIdxs_next[(j+1)%ds],detourIdxs_next[j],detourIdxs_this[(j+1)%ds]);
        }
        for(int j=0;j<(int)detourIdxs_this.size();j++){
            detourIdxs_this[j]=detourIdxs_next[j];
        }
    }
    int ci = vertices_ori.size()/3;
    pushVertice_ori(c);
    pushNormal(n);
    pushColor(QVector3D(0.5f,0.5f,0.5f));
    int ds = (int)detourIdxs_this.size();
    for(int i=0;i<ds;i++){
        pushIndice(detourIdxs_this[i],detourIdxs_this[(i+1)%ds],ci);
    }
    connectorFaceIdxs.push_back(ci);
}
void ModelManager::regenByPlateIntersec(){//must use model vertices, check all vertices;
    if(selecIdxs.size()<3)return;
    std::vector<QVector3D> s = getSelecPointsByIdxs_ori();
//    edges.clear();
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
            float d1 = absf(pointDistanceToPlane(v[i],s[0],s[1],s[2]));
            float d2 = absf(pointDistanceToPlane(v[(i+1)%3],s[0],s[1],s[2]));
            if(isCrossPlane(v[i], v[(i+1)%3], s[0], s[1], s[2])){
                v[flag]=(v[(i+1)%3]-v[i])*d1/(d1+d2) + v[i];
                cl[flag-3] = (getColor(indices[j+i])+getColor(indices[j+(i+1)%3]))/2;
                nm[flag-3] = (getNormal(indices[j+i])+getNormal(indices[j+(i+1)%3]))/2;
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
    /*
    for(int j=ids;j<(int)indices.size();j+=3){
        colors[indices[j]*3]=0.0f;colors[indices[j]*3+1]=1.0f;colors[indices[j]*3+2]=0.0f;
        colors[indices[j+1]*3]=0.0f;colors[indices[j+1]*3+1]=1.0f;colors[indices[j+1]*3+2]=0.0f;
        colors[indices[j+2]*3]=0.0f;colors[indices[j+2]*3+1]=1.0f;colors[indices[j+2]*3+2]=0.0f;
    }
    for(int j=vts;j<(int)vertices_ori.size();j++){
        colors[j] = 0.0f;
    }
    */
}

void ModelManager::pullConnect(){
    if(connectorFaceIdxs.size()==0)return;
    QVector3D n = detourNormal_ori();
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
    QVector3D n = detourNormal_ori();
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

QVector3D ModelManager::detourNormal(){
    QVector3D c(0,0,0);
    std::vector<QVector3D> detourPoints;
    for(int i=0;i<(int)detourIdxs.size();i++){
        detourPoints.push_back(getVertice(detourIdxs[i]));
        c += detourPoints[i];
    }
    c /= (float)detourPoints.size();
    QVector3D n(0,0,0);
    for(int i=1;i<(int)detourPoints.size();i++){
        n += QVector3D::crossProduct(detourPoints[i-1]-c, detourPoints[i]-c);
    }
    return n.normalized();
}

QVector3D ModelManager::selecPointsNormal(){
    std::vector<QVector3D> s = getSelecPointsByIdxs();
    QVector3D c = (s[0]+s[1]+s[2])/3;
    QVector3D n = QVector3D::crossProduct(s[0]-c,s[1]-c);
    return n;
}

QVector3D ModelManager::detourNormal_ori(){
    QVector3D c(0,0,0);
    std::vector<QVector3D> detourPoints;
    for(int i=0;i<(int)detourIdxs.size();i++){
        detourPoints.push_back(getVertice_ori(detourIdxs[i]));
        c += detourPoints[i];
    }
    c /= (float)detourPoints.size();
    QVector3D n(0,0,0);
    for(int i=1;i<(int)detourPoints.size();i++){
        n += QVector3D::crossProduct(detourPoints[i-1]-c, detourPoints[i]-c);
    }
    return n.normalized();
}

QVector3D ModelManager::selecPointsNormal_ori(){
    std::vector<QVector3D> s = getSelecPointsByIdxs_ori();
    QVector3D c = (s[0]+s[1]+s[2])/3;
    QVector3D n = QVector3D::crossProduct(s[0]-c,s[1]-c);
    return n;
}
void ModelManager::reverseDetours(){
    int ds = (int)detourIdxs.size();
    for(int i=1;i<(ds+1)/2;i++){
        unsigned int t = detourIdxs[i];
        detourIdxs[i] = detourIdxs[ds-i];
        detourIdxs[ds-i] = t;
    }
}
QVector3D ModelManager::selecPointsCenter(){
    std::vector<QVector3D> s = getSelecPointsByIdxs();
    return (s[0]+s[1]+s[2])/3;
}
QVector3D ModelManager::selecPointsCenter_ori(){
    std::vector<QVector3D> s = getSelecPointsByIdxs_ori();
    return (s[0]+s[1]+s[2])/3;
}
QVector3D ModelManager::detourCenter(){
    QVector3D c(0,0,0);
    for(int i=0;i<detourIdxs.size();i++){
        QVector3D v = getVertice(detourIdxs[i]);
        c+=v;
    }
    return c/detourIdxs.size();
}
QVector3D ModelManager::detourCenter_ori(){
    QVector3D c(0,0,0);
    for(int i=0;i<detourIdxs.size();i++){
        QVector3D v = getVertice_ori(detourIdxs[i]);
        c+=v;
    }
    return c/detourIdxs.size();
}
