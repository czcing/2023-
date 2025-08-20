#include"game.h"
game::game(int n):maze(n),dpmove(n),greedymove(n){}
game::game(string filename):maze(filename),dpmove(filename),greedymove(filename){}
void game::gaming()
{    
    cout<<"1.Greedy"<<endl;
    cout<<"2.DynamicPrograming"<<endl;
    cout<<"3.exit(0)"<<endl;
    int choice;
    cin>>choice;
    cin.ignore();
    switch(choice)
    {
        case 1:
        {
            Greedy();
            gamepath=greedy_path;
            break;
        }
        case 2:
        {
           DynamicProgramming();
           gamepath=dp_path[end.first][end.second];
           break;
        }
        case 3:
        {
            exit(0);
            break;
        }
    }
    system("pause");
    system("cls");
    cout<<"path:"<<endl;
    for(int i=0;i<gamepath.size();i++){
        cout<<"("<<gamepath[i].first<<","<<gamepath[i].second<<")"<<" ";
    }
    system("pause");
    system("cls");
    cout<<endl;
    for(int i=0;i<gamepath.size();i++)
    {
        char ch=mazegraph[gamepath[i].first][gamepath[i].second];
        mazegraph[gamepath[i].first][gamepath[i].second]='M';
        printmaze();
        mazegraph[gamepath[i].first][gamepath[i].second]=' ';
        printmaze_visited[gamepath[i].first][gamepath[i].second]=true;
        cout<<"POS:"<<"("<<gamepath[i].first<<","<<gamepath[i].second<<")"<<" ";
        switch(ch)
        {
            case 'S':
            {
               cout<<"This is start!";
               break;
            }
            case 'E':
            {
                cout<<"This is end!";
                break;
            }
            case 'T':
            {
                if(find(gamepath.begin(),gamepath.begin()+i,gamepath[i])==gamepath.begin()+i){
                    value-=30;
                    cout<<"This is trap,scored -30!";
                }else{
                    cout<<"This is trap,but already passed!";
                }
                break;
            }
            case 'L':
            {
                if(find(gamepath.begin(),gamepath.begin()+i,gamepath[i])==gamepath.begin()+i){
                    cout<<"This is locler,start to break the locker!";
                    password_lock(value);
                }else{    
                    cout<<"This is locker,but already passed!";
                }
                break;
            }
            case 'G':
            {
                if(find(gamepath.begin(),gamepath.begin()+i,gamepath[i])==gamepath.begin()+i){
                    value+=50;
                    cout<<"This is resource,scored +50!";
                }else{
                    cout<<"This is resource,but already passed!";
                }
                    break;
            }
            case 'B':
            {   if(find(gamepath.begin(),gamepath.begin()+i,gamepath[i])==gamepath.begin()+i){
                    cout<<"This is boss,start to fight!";
                    value-=boss_fighting();
                    
                }else{
                    cout<<"This is boss,but already passed!";
            }
                break;
            }
            case ' ':
            {
                cout<<"This is road!";
                break;
            }
        }
        cout<<" The score is:"<<value<<endl;
        system("pause");
        system("cls");
    }
     system("cls");
    cout<<"The game END"<<endl;
    cout<<"The final path length:"<<gamepath.size()<<endl;
    cout<<"The final score:"<<value<<endl;
    system("pause");
   
}