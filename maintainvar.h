#ifndef MAINTAINVAR_H
#define MAINTAINVAR_H
#include <skelmanager.h>
class MaintainVar{

public:
    MaintainVar(){}
    SkelManager skelMgr;
    int tarObj = 0;
    int assistPlaneIdx = 0;
    bool quickmode=true;
    float normalizeVal = 100.0f;
    int totalconnectType=2;
    int connectType = 0;
    bool connectPuted = false;
    int connectTarNum = 0;
    int cutcnt=0;
    std::vector<QVector3D> cuttingpoint;
    std::vector<QVector3D> cuttingpnorm;
    std::vector<QVector3D> cuttingpoint2;
    std::vector<QVector3D> cuttingpnorm2;
    std::vector<bool> IsReverseN;
    bool locked=false;
    void lock(){locked=true;}
    void unlock(){if(!splock)locked=false;}
    bool splock = false;
    int cmpstate = 0;
};

#endif // MAINTAINVAR_H
