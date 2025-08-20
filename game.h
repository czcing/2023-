
#ifndef _GAME_H_
#define _GAME_H_
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include"dpmove.h"
#include"greedymove.h" 
#include<limits>
#include<windows.h>
#include"boss.h"
#include"locker.h"
using namespace std;
class game:public dpmove,public greedymove
{
private:
    vector<pair<int,int>> gamepath;
    int value=0;
public:
    game(int n);
    void gaming();
    game(string filename);
};

#endif