// nation.h
#ifndef NATION_H
#define NATION_H

#define SORT_BY_TOTAL 1
#define SORT_BY_MEN 2
#define SORT_BY_WOMEN 3
#define SORT_BY_ID 4

#include<qDebug>

class nation {
private:
    //                           0  1  2  3  4  5
    const int RANKRULE[2][6]  = {0, 5, 3, 2, 0, 0,
                                 0, 7, 5, 3, 2, 1};

    // char name[100] = { 0 };
    int ID;
    int* projects;
    int totalScore=0;
    int menScore=0;
    int womenScore=0;
    int m, w;

public:
    nation() :m(0), w(0), projects(nullptr) {}

    ~nation() {
        delete[] projects;
    }

    void init(const int seq, int _m, int _w);
    void setRank(int projectSeq, int rank);
    void calcScore(bool *&rankRule);

    // return a > b
    static bool compareScore(const nation& a, const nation& b, int method);
    int getID();
    int getTotalScore();
    int getMenScore();
    int getWomenScore();
    int getRank(int i);
};

#endif // NATION_H
