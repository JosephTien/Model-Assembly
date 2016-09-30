#ifndef SKELMANAGER_H
#define SKELMANAGER_H

#include "pch.h"

class SkelManager{

public :
    SkelManager(){}
    std::vector<QVector3D> skel;
    std::string path;
    bool loaded=false;
    std::string label[14] = {"body"
                            , "neck", "shoulder_left", "shoulder_right", "wist"
                            , "head", "elbow_left", "elbow_right", "knee_left", "knee_right"
                            , "hand_left", "hand_right", "feet_left", "feet_right"};
    int tarByName(std::string name){
        for(int i=0;i<14;i++){
            if(label[i].compare(name)==0){
                return i;
            }
        }
    }
    QVector3D partPosition(std::string name){
        return skel[tarByName(name)];
    }
    SkelManager(std::string path){
        load(path);
    }
    void load(std::string path){
        skel.clear();
        FILE* fin=fopen(path.data(),"r");
        float valx,valy,valz;
        while(fscanf(fin,"%f",&valx)!=EOF){
            fscanf(fin,"%f",&valy);
            fscanf(fin,"%f",&valz);
            skel.push_back(QVector3D(valx,valy,valz));
            //std::cout<< valx << " "  << valy<< " " << valz << std::endl;
        }
        fclose(fin);
        loaded = true;
    }
    void Reset(){
        skel.clear();
        path = "";
        loaded=false;
    }
    void getPartsInfo(std::vector<QVector3D> &c, std::vector<QVector3D> &n){
        c.clear();
        n.clear();
        c.push_back((partPosition("neck")+partPosition("wist"))/2);
        n.push_back((partPosition("neck")-partPosition("wist")));

        /********************************************************************************/

        c.push_back(partPosition("head"));
        n.push_back((partPosition("head")-partPosition("neck"))*2);

        c.push_back((partPosition("elbow_left")+partPosition("shoulder_left"))/2);
        n.push_back((partPosition("elbow_left")-partPosition("shoulder_left")));

        c.push_back((partPosition("elbow_right")+partPosition("shoulder_right"))/2);
        n.push_back((partPosition("elbow_right")-partPosition("shoulder_right")));

        c.push_back((partPosition("knee_left")+partPosition("wist"))/2);
        n.push_back((partPosition("knee_left")-partPosition("wist")));

        c.push_back((partPosition("wist")+partPosition("knee_right"))/2);
        n.push_back((partPosition("wist")-partPosition("knee_right")));

        /********************************************************************************/

        c.push_back((partPosition("hand_left")+partPosition("elbow_left"))/2);
        n.push_back((partPosition("hand_left")-partPosition("elbow_left")));

        c.push_back((partPosition("hand_right")+partPosition("elbow_right"))/2);
        n.push_back((partPosition("hand_right")-partPosition("elbow_right")));

        c.push_back((partPosition("feet_left")+partPosition("knee_left"))/2);
        n.push_back((partPosition("feet_left")-partPosition("knee_left")));

        c.push_back((partPosition("feet_right")+partPosition("knee_right"))/2);
        n.push_back((partPosition("feet_right")-partPosition("knee_right")));

    }
    void getCutInfo(std::vector<QVector3D> &c, std::vector<QVector3D> &n){
        QVector3D temp1,temp2;
        c.clear();
        n.clear();

        c.push_back(partPosition("neck"));
        temp1 = (partPosition("neck")-partPosition("body")).normalized();
        temp2 = (partPosition("head")-partPosition("neck")).normalized();
        n.push_back((temp1+temp2)/2);

        c.push_back(partPosition("shoulder_left"));
        temp1 = (partPosition("shoulder_left")-partPosition("shoulder_right")).normalized();
        n.push_back(temp1);

        c.push_back(partPosition("shoulder_right"));
        temp1 = (partPosition("shoulder_right")-partPosition("shoulder_left")).normalized();
        n.push_back(temp1);

        c.push_back(partPosition("wist"));
        temp1 = (partPosition("wist")-partPosition("neck")).normalized();
        n.push_back(temp1);

        c.push_back(partPosition("elbow_left"));
        temp1 = (partPosition("elbow_left")-partPosition("shoulder_left")).normalized();
        temp2 = (partPosition("hand_left")-partPosition("elbow_left")).normalized();
        n.push_back((temp1+temp2)/2);

        c.push_back(partPosition("elbow_right"));
        temp1 = (partPosition("elbow_right")-partPosition("shoulder_right")).normalized();
        temp2 = (partPosition("hand_right")-partPosition("elbow_right")).normalized();
        n.push_back((temp1+temp2)/2);

        c.push_back(partPosition("knee_left"));
        temp1 = (partPosition("feet_left")-partPosition("knee_left")).normalized();
        n.push_back(temp1);

        c.push_back(partPosition("knee_right"));
        temp1 = (partPosition("feet_right")-partPosition("knee_right")).normalized();
        n.push_back(temp1);
    }

};

#endif // SKELMANAGER_H
