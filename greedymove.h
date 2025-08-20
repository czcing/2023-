
#ifndef _GREEDYMOVE_H_
#define _GREEDYMOVE_H_
#include"maze.h"
#include"algorithm"
#include<climits>
using namespace std;
class greedymove:virtual public maze
{
protected:
    vector<vector<double>> greedy_maze_value;
    const vector<pair<int,int>> greedy_directions = {{-1,0}, {1,0}, {0,-1}, {0,1},{1,1},{-1,-1},{1,-1},{-1,1}};
    vector<pair<int,int>> greedy_path{vector<pair<int,int>>(1000000, {0, 0})};
    vector<pair<int,int>> temp_greedy_path;
    int greedy_move_value=-10000;
    int temp_greedy_move_value=0;  
public:
    void Greedy();
    void greedy();
    greedymove(int n);
    greedymove(string filename);
};

#endif