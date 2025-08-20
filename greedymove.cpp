#include"greedymove.h"
void greedymove::greedy()
{
    pair<int,int> pos;
    temp_greedy_path.push_back(start);   
    pos=start;
    pair<int,int> temppos={-1,-1};
    while(1){
        double value=-10;
        map<int,vector<pair<int,int>>> mp;
        for(auto dir:greedy_directions){
            int x=pos.first+dir.first;
            int y=pos.second+dir.second;
            if(x>=0 && x<n && y>=0 && y<n && mazegraph[x][y]!='#'){ 
                if(dir.first==0 && dir.second==1||dir.first==1 && dir.second==0||dir.first==-1 && dir.second==0||dir.first==0 && dir.second==-1)
                {
                    double newval=greedy_maze_value[x][y];
                    if(newval>=value){
                        mp[newval].push_back({x,y});
                        value=newval;
                    }
                    if(mazegraph[x][y]=='T')
                    {
                        greedy_maze_value[x][y]+=0.9;
                    }
                }else{
                    double newval;
                    int val1=greedy_maze_value[x-dir.first][y];
                    int val2=greedy_maze_value[x][y-dir.second];
                    int val=greedy_maze_value[x][y];
                    if(val1>val2){
                        newval=val1+val-1;
                        if(newval>=value){
                            mp[newval].push_back({x-dir.first,y});
                            value=newval;
                        }
                    }else if(val2>val1){
                        newval=val2+val-1;
                        if(newval>=value){
                            mp[newval].push_back({x,y-dir.second});
                            value=newval;
                        }
                    }else if(val1!=-10000&&val2!=-10000&&val1==val2){
                        newval=val1+val-1;
                        if(newval>=value){
                            mp[newval].push_back({x,y-dir.second});
                            mp[newval].push_back({x-dir.first,y});
                            value=newval;
                        }
                    }else{
                        continue;
                    }
                }
            }else
                continue;
        }
        if(mp.empty())
        {
            cout<<"no greedy_path found"<<endl;
            exit(0);
        }/*
        cout<<temppos.first<<' '<<temppos.second<<endl;
        cout<<'('<<mp[value][0].first<<','<<mp[value][0].second<<")"<<endl;
        cout<<'('<<mp[value][1].first<<','<<mp[value][1].second<<")"<<endl;*/
        if(find(mp[value].begin(), mp[value].end(), temppos) != mp[value].end()&&mp[value].size()>1)
        mp[value].erase(remove(mp[value].begin(), mp[value].end(), temppos), mp[value].end());
        /*cout<<'('<<mp[value][0].first<<','<<mp[value][0].second<<")"<<endl;
        cout<<'('<<mp[value][1].first<<','<<mp[value][1].second<<")"<<endl;*/
        temppos=pos;
        pos=mp[value][random_int(0,mp[value].size()-1)];
        temp_greedy_move_value+=value;
        greedy_maze_value[pos.first][pos.second]=-0.2;
        temp_greedy_path.push_back(pos);
        if(pos==end){
            return;
        }
    }
}
void greedymove::Greedy()
{
    greedy_maze_value.assign(n, vector<double>(n));
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++){
            greedy_maze_value[i][j]=mazevalue[i][j];
        }
    }
    for(int i=0;i<n*10;i++){
        greedy();
        if(temp_greedy_move_value==greedy_move_value&&temp_greedy_path.size()<greedy_path.size()||temp_greedy_move_value>greedy_move_value)
        {
            greedy_path=temp_greedy_path;
            greedy_move_value=temp_greedy_move_value;
        }
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++){
                greedy_maze_value[i][j]=mazevalue[i][j];
            }
        }
        temp_greedy_move_value=0;
        temp_greedy_path.clear();
    }
    cout<<endl;
}
greedymove::greedymove(int n):maze(n){}  
greedymove::greedymove(string filename):maze(filename){}