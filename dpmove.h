
#ifndef _DPMOVE_H_
#define _DPMOVE_H_
#include "maze.h"
#include<algorithm>
using namespace std;
class dpmove:virtual public maze{
protected:
    typedef struct node{
        pair<int, int> point;
        int dp;
        vector<pair<int,int>> path;
    }Node;
    vector<vector<double>> dp;
    vector<vector<int>> visittime;
    int findtime(Node cur,int x,int y);
    vector<vector<vector<pair<int,int>>>> dp_path;
    queue<Node> pq;
    void init();
public:
    dpmove(int n);
    dpmove(string filename);
    void printdp_path();
    void DynamicProgramming();
};

#endif