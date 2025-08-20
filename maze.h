
#ifndef _MAZE_H_
#define _MAZE_H_
#include<iostream>
#include<vector>
#include<random>
#include<map>
#include<fstream>
#include"json.hpp"
#include<queue>
using namespace std;
class maze
{
protected:
    const vector<pair<int,int>> directions = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    vector<vector<char>> mazegraph;
    pair<int,int> start;
    pair<int,int> end;
    int trap;
    int locker;
    int resourse;
    int n;
    vector<vector<int>> mazevalue;
    vector<vector<bool>> visited;
    vector<vector<bool>> printmaze_visited{vector<vector<bool>>(n, vector<bool>(n,false))};
    int getValue(int x, int y);
    void readjson(string filename);
    void printmaze();
    void addelement();
    void mazeinit(int x1,int x2,int y1,int y2);
    int random_int(int i, int j);
    void add_spe_ele(int ele_num,int ele_type);
    bool detect_isolated_point();   
    void bfs(int x,int y,queue<pair<int,int>> &q);
    void initvalue();
    void saveVectorToJson(string filename);
    bool has_unique_path();
    bool dfs(int x,int y,int& path_count);
public:
    maze(string filename);
    maze(int n);
};

#endif

