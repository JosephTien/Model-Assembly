#include "modelmanager.h"
#include "pch.h"
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
void ModelManager::gendetourPlane_convex(){
    gendetourPlane_convex(detourIdxs);
    while(detourIdxsQueue_flag < detourIdxsQueue.size()){
        gendetourPlane_convex(detourIdxsQueue[detourIdxsQueue_flag]);
        detourIdxsQueue_flag++;
    }
    for(int i=detourIdxsList.size()-1;i>0;i--){
        detourIdxs = detourIdxsList[i];
//        gendetourPlane(detourCenterList[i]);
        gendetourPlane_simple(detourCenterList[i]);
    }
    detourIdxs = detourIdxsList[0];
    gendetourPlane(detourCenterList[0]);

    detourIdxsQueue.clear();
    detourIdxsList.clear();
    detourCenterList.clear();
}
void ModelManager::gendetourPlane_convex(std::vector<int> detourIdxs_this){
    LoopList looplist;
    looplist.assign(detourIdxs_this);
    QVector3D planeNorm  = detourNormal_ori(detourIdxs_this);
    QVector3D centerVert = detourCenter_ori(detourIdxs_this);
    LoopList::ListNode * endi = looplist.curNode;
    int cnt=looplist.size();
    bool prevbad=false;
    do{
        QVector3D iv = getVertice_ori(looplist.getCurIdx());
        LoopList::ListNode * j    = looplist.curNode->next;
        LoopList::ListNode * endj = looplist.curNode;
        bool bad=false;
        while(j->next!=endj){
            QVector3D jv1 = getVertice_ori(j->idx);
            QVector3D jv2 = getVertice_ori(j->next->idx);
            QVector3D ivc = centerVert-iv;
            QVector3D ivjv1 = jv1-iv;
            QVector3D ivjv2 = jv2-iv;
            QVector3D t1 = QVector3D::crossProduct(ivjv2,ivc);
            QVector3D t2 = QVector3D::crossProduct(ivc,ivjv1);
            QVector3D t3 = QVector3D::crossProduct(ivjv2,ivjv1);
            if(dotProduct(t1,t2)>0 && ivc.length()>ivjv1.length()){
                bad=true;
                break;
            }
            j=j->next;
        }
        if(bad){
            if(!prevbad){
                detourIdxsQueue.push_back(std::vector<int>());
            }
            detourIdxsQueue[detourIdxsQueue.size()-1].push_back(looplist.getCurIdx());
            looplist.deleteNode();
        }
        prevbad=bad;
        looplist.forward();
        cnt--;
    }while(cnt>0);

//    for(int i=0;i<looplistQueue.size();i++){
//        detourIdxs.clear();
//        detourIdxs = looplistQueue[i];
//        gendetourPlane();
//    }

//    detourIdxs.clear();
//    detourIdxs = looplist.getIdxs();
//    gendetourPlane(centerVert);
    detourIdxsList.push_back(looplist.getIdxs());
    detourCenterList.push_back(centerVert);
    looplist.clear();
}

void ModelManager::gendetourPlane_patch(){
    LoopList looplist;
    looplist.assign(detourIdxs);
    QVector3D planeNorm = detourNormal_ori();
    QVector3D centerVert = detourCenter_ori();
    int ds = detourIdxs.size();
    float d=0;
    for(int i=1;i<ds;i++){
        d+=(getVertice_ori(detourIdxs[i])-getVertice_ori(detourIdxs[i-1])).length();
    }d/=ds;
    int thre1 = 5;
    //while(looplist.cnt>100){
    for(int cnt=0;cnt<1500 && looplist.cnt>ds/5;cnt++){
        unsigned int prevIdx=looplist.getPrevIdx();
        unsigned int curIdx =looplist.getCurIdx();
        unsigned int nextIdx=looplist.getNextIdx();
        QVector3D prevVert=getVertice_ori(prevIdx);
        QVector3D curVert=getVertice_ori(curIdx);
        QVector3D nextVert=getVertice_ori(nextIdx);
        QVector3D prevVec = curVert-prevVert;
        QVector3D curVec = nextVert-curVert;
        QVector3D crossVec = nextVert-prevVert;
        QVector3D centerVec = centerVert-curVert;
        if(dotProduct( QVector3D::crossProduct(prevVec,curVec), planeNorm)>0){
            if(false){
//            }else if(crossVec.length()<=curVec.length()&&crossVec.length()<=prevVec.length()){
//                pushIndice(prevIdx,curIdx,nextIdx);
//                looplist.deleteNode();
//                looplist.forward();
            }else if(acosf(dotProduct(-prevVec.normalized(),curVec.normalized())) < M_PI/2){//90度
                pushIndice(prevIdx,curIdx,nextIdx);
                looplist.deleteNode();
                looplist.forward();
//            }else if(curVec.length()/prevVec.length() < 1/thre1){
//                pushIndice(prevIdx,curIdx,nextIdx);
//                looplist.deleteNode();
//                looplist.forward();
//            }else if(curVec.length()/prevVec.length() > thre1){
//                pushIndice(prevIdx,curIdx,nextIdx);
//                looplist.deleteNode();
//                looplist.forward();
            }else{
                QVector3D c1 = (prevVert+curVert)/2;
                QVector3D c2 = (curVert+nextVert)/2;
                QVector3D n1 = QVector3D::crossProduct(planeNorm,prevVec).normalized();
                QVector3D n2 = QVector3D::crossProduct(planeNorm,curVec).normalized();
                float t = ((n1.y()*(c1-c2).x())-n1.x()*(c1-c2).y())/(n2.x()*n1.y()-n2.y()*n1.x());
                QVector3D co = c2+t*n2;
                QVector3D no = (co-curVert).normalized();

                QVector3D newVert = no*d + curVert;
                unsigned int newIdx = pushVertice_ori(newVert);
                pushNormal(planeNorm);
                pushColor(QVector3D(0.5f,0.5f,0.5f));
                pushIndice(prevIdx,curIdx,newIdx);
                pushIndice(newIdx,curIdx,nextIdx);
                looplist.deleteNode();
                looplist.insertNode(newIdx);
                looplist.forward();
//            }else{
//                if(dotProduct(QVector3D::crossProduct(centerVec,-prevVec),QVector3D::crossProduct(curVec,centerVec))>0){
//                    QVector3D newVert = centerVec.normalized()*d + curVert;
//                    //QVector3D newVert = curVec - prevVec + curVert;
//                    unsigned int newIdx = pushVertice_ori(newVert);
//                    pushNormal(planeNorm);
//                    pushColor(QVector3D(0.5f,0.5f,0.5f));
//                    pushIndice(prevIdx,curIdx,newIdx);
//                    pushIndice(newIdx,curIdx,nextIdx);
//                    looplist.deleteNode();
//                    looplist.insertNode(newIdx);
//                    looplist.forward();
//                }
            }
        }
        looplist.forward();
    }
}
void ModelManager::gendetourPlane(){
    QVector3D c = detourCenter_ori();
    gendetourPlane(c);
}
bool specialDense = false;
void ModelManager::gendetourPlane(QVector3D c){//assume sorted, problem of go through
    std::vector<QVector3D> detourPoints;
    std::vector<QVector3D> deformPoints;
    std::vector<unsigned int> detourIdxs_this;
    std::vector<unsigned int> detourIdxs_next;
    for(int i=0;i<(int)detourIdxs.size();i++){
        detourIdxs_this.push_back(detourIdxs[i]);
        QVector3D v = getVertice_ori(detourIdxs[i]);
        detourPoints.push_back(v);
    }
    QVector3D n = detourNormal_ori();
    float d=0,r=0;
    for(int i=1;i<(int)detourPoints.size();i++){
        d+=(detourPoints[i-1]-detourPoints[i]).length();
        r+=(c-detourPoints[i]).length();
    }
    int rings=((int)(r/d))-1;
    if(specialDense)rings=1000;
    connectorFaceIdxs.clear();
    float interval = 0.5f;
    bool simp=true;
    float divcnt = 0.5;
    for(int i=0;i<rings;i++){
        int vs = (int)vertices_ori.size()/3;
        int ds = (int)detourIdxs_this.size();
        detourIdxs_next.clear();
        simp = false;
        if((rings+1-i)<=(rings+1)*divcnt){
            divcnt/=2;
            simp = true;
        }
        if(!simp){
            for(int j=0;j<ds;j++){
                QVector3D vo = getVertice_ori(detourIdxs_this[j]);
                QVector3D vb = getVertice_ori(detourIdxs_this[(j+ds-1)%ds]);
                QVector3D vf = getVertice_ori(detourIdxs_this[(j+1)%ds]);
                QVector3D voc = c - vo;
                QVector3D vn = voc/(rings+1.0f-i);
                QVector3D v = vo + vn;
                pushVertice_ori(v);
                pushNormal(n);
                pushColor(QVector3D(0.5f,0.5f,0.5f));
                detourIdxs_next.push_back(vs+j);
                if(i>(int)(rings*interval)){
                    connectorFaceIdxs.push_back(vs+j);
                }
            }
            for(int j=0;j<ds;j++){
                pushIndice(detourIdxs_this[j],detourIdxs_this[(j+1)%ds],detourIdxs_next[j]);
                pushIndice(detourIdxs_next[(j+1)%ds],detourIdxs_next[j],detourIdxs_this[(j+1)%ds]);
            }
        }else{
            for(int j=1;j<ds;j+=2){
                QVector3D vo = getVertice_ori(detourIdxs_this[j]);
                QVector3D v = vo + (c-vo)/(rings+1.0f-i);
                pushVertice_ori(v);
                pushNormal(n);
                pushColor(QVector3D(0.5f,0.5f,0.5f));
                detourIdxs_next.push_back(vs+j/2);
                if(i>(int)(rings*interval)){
                    connectorFaceIdxs.push_back(vs+j/2);
                }
            }
            int j=1;
            for(;j<ds;j+=2){
                pushIndice(detourIdxs_this[j-1],detourIdxs_this[j],detourIdxs_next[j/2]);
                pushIndice(detourIdxs_this[j],detourIdxs_this[(j+1)%ds],detourIdxs_next[j/2]);
                if(j/2>0){
                    pushIndice(detourIdxs_this[j-1],detourIdxs_next[j/2],detourIdxs_next[j/2-1]);
                }
            }
            j-=2;
            if(j==ds-2){
                pushIndice(detourIdxs_next[0],detourIdxs_next[j/2],detourIdxs_this[j+1]);
                pushIndice(detourIdxs_next[0],detourIdxs_this[j+1],detourIdxs_this[0]);
            }else if(j==ds-1){
                pushIndice(detourIdxs_next[0],detourIdxs_next[j/2],detourIdxs_this[0]);
            }

        }

        detourIdxs_this.clear();
        for(int j=0;j<(int)detourIdxs_next.size();j++){
            detourIdxs_this.push_back(detourIdxs_next[j]);
        }
        if(detourIdxs_this.size()<6)break;
    }
        int ci = vertices_ori.size()/3;
        pushVertice_ori(c);
        pushNormal(n);
        pushColor(QVector3D(0.5f,0.5f,0.5f));
        int dts = (int)detourIdxs_this.size();
        for(int i=0;i<dts;i++){
            pushIndice(detourIdxs_this[i],detourIdxs_this[(i+1)%dts],ci);
        }
        connectorFaceIdxs.push_back(ci);
}

void ModelManager::gendetourPlane_simple(QVector3D c){
    //QVector3D c = detourCenter_ori();
    QVector3D n = detourNormal_ori();
    int ci = vertices_ori.size()/3;
    pushVertice_ori(c);
    pushNormal(n);
    pushColor(QVector3D(0.5f,0.5f,0.5f));
    int ds = (int)detourIdxs.size();
    for(int i=0;i<ds;i++){
        pushIndice(detourIdxs[i],detourIdxs[(i+1)%ds],ci);
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
    gendetourPlane();//gendetourPlane_convex();//fillByDetour();
    connectorNormal_ori = detourNormal_ori();
    connectorCenter_ori = detourCenter_ori();
    connectorRadii_ori=FLT_MAX;
    for(int i=0;i<detourIdxs.size();i++){
        connectorRadii_ori = std::min(connectorRadii_ori,(getVertice_ori(detourIdxs[i])-connectorCenter_ori).length());
    }
    regenNormals();
    setColors(0.5f,0.5f,0.5f);
    applyed=false;applyModelMatrix();
}

/*
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
//        //sort version
//        //for(int j=0;j<(int)neighbor[i].size();i++){
//        //    unsigned int t = neighbor[i][j];
//        //    for(int k=j-1;k>=0;k--){
//        //        if(neighbor[i][k]>t){
//        //           neighbor[i][k+1] = neighbor[i][k];
//        //        }else{
//        //           neighbor[i][k+1] = t;
//        //           break;
//        //        }
//        //    }
//        //}
//
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
*/
/*
int ModelManager::calDetourByPlane(float radii){
    if(selecIdxs.size()<3)return 0;
    regenByPlateIntersec();//detourIdxs.clear();
    calNeighbor();
    std::vector<QVector3D> s;
    s = getSelecPointsByIdxs();
    QVector3D c  = (s[0]+s[1]+s[2])/3;
    bool * visited;visited = (bool*)malloc(sizeof(bool)*vertices.size());memset(visited,false,vertices.size());
    for(int i=0;i<(int)vertices.size()/3;i++){
        if(std::fabs(pointDistanceToPlane(getVertice(i),s[0],s[1],s[2]))<minThre){
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

//    bool * visited;visited = (bool*)malloc(sizeof(bool)*vertices.size());memset(visited,false,vertices.size());
//    int curIdx = selecIdxs[0];
//    int cnt=0;
//    while(true){
//        std::cout << cnt++ << std::endl;
//        boolean valid=false;
//        for(int i=0;i<(int)neighbor[curIdx].size();i++){
//            int tarIdx = neighbor[curIdx][i];
//            float val = std::fabs(pointDistanceToPlane(getVertice(curIdx),s1,s2,s3));
//            if(val>minThre)continue;
//            if(visited[tarIdx])continue;

//            std::cout << val << std::endl;
//            valid=true;
//            curIdx = tarIdx;
//            detourIdxs.push_back(curIdx);
//            visited[curIdx]=true;
//            break;
//        }
//        if(!valid)break;
//    }

//    if(curIdx==selecIdxs[0]){
//        std::cout << "well detour" << std::endl;
//        return 1;
//    }else{
//        std::cout << "detour failed" << std::endl;
//        return 0;
//    }
}
*/
