// nation.cpp
#include "nation.h"

void nation::init(const int seq, int _m, int _w){
    m = _m;
    w = _w;
    projects = new int[m+w];
    ID = seq+1;
    for(int i=0;i<m+w;i++) projects[i]=0;
}

void nation::setRank(int projectSeq, int rank){
    projects[projectSeq] = rank;
}

void nation::calcScore(bool *&rankRule){
    // traverse i projects
    for(int i=0;i<m;i++)
        menScore += RANKRULE[rankRule[i]][projects[i]];
    for(int i=m;i<m+w;i++)
        womenScore += RANKRULE[rankRule[i]][projects[i]];

    totalScore = menScore + womenScore;
}

bool nation::compareScore(const nation& a, const nation& b, int method) {
    if(SORT_BY_TOTAL == method){
        if(a.totalScore == b.totalScore) return a.ID < b.ID;
        else return a.totalScore > b.totalScore;
    }
    if(SORT_BY_MEN == method){
        if(a.menScore == b.menScore) return a.ID < b.ID;
        else return a.menScore > b.menScore;
    }
    if(SORT_BY_WOMEN == method){
        if(a.womenScore == b.womenScore) return a.ID < b.ID;
        else return a.womenScore > b.womenScore;;
    }

    if(SORT_BY_ID == method)    return a.ID < b.ID;

    return 0;
}

int nation::getID(){return ID;}
int nation::getTotalScore(){return totalScore;}
int nation::getMenScore(){return menScore;}
int nation::getWomenScore(){return womenScore;}
int nation::getRank(int i){return projects[i];}
