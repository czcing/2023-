#include "game.h"
using namespace std;
int main(){
    cout<<"mazegame"<<endl;
    cout<<"1.random init maze"<<endl;
    cout<<"2.read json maze"<<endl;
    int n;
    cin>>n;
    switch(n){
        case 1:
        {
            int size;
            cout<<"please scanf size of maze:"<<endl;
            cin>>size;
            game g(size);
            g.gaming();
            break;
        }
        case 2:
        {
            game g("mazegraph1.json");
            g.gaming();
            break;
        }
    }

}