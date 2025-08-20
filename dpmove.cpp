#include "dpmove.h"
void dpmove::init()
{
    dp_path.assign(n, vector<vector<pair<int,int>>>(n));
    visittime.assign(n, vector<int>(n));
    for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
        if(mazegraph[i][j]=='#') continue;
        int count=0;
        for(auto &dir:directions){
            int ni=i+dir.first;
            int nj=j+dir.second;
            if(ni>=0 && ni<n && nj>=0 && nj<n&&mazegraph[ni][nj]!='#'){
                count++;
            }
            visittime[i][j]=count;
        }
    }
    }
    cout<<endl;
    for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        dp_path[i][j].resize(n * n, {start.first, start.second});  // 动态调整大小并填充
    }
}
    dp=vector<vector<double>>(n, vector<double>(n, INT_MIN));
}
void dpmove::printdp_path()
{
    cout<<"dpmax:"<<dp[end.first][end.second]<<endl;
    cout<<endl;
    cout<<"dp_pathlen:"<<dp_path[end.first][end.second].size()<<endl;
    cout<<"dp_path:"<<endl;
    for(auto &p:dp_path[end.first][end.second]){
        cout<<'('<<p.first<<','<<p.second<<')'<<"->";
    }
    cout<<endl;
}
void dpmove::DynamicProgramming(){
    init();
    pq.push({{start.first, start.second},0,{{start.first, start.second}}});
    while(!pq.empty()){
        Node cur=pq.front(); 
        pq.pop();
        int i=cur.point.first;
        int j=cur.point.second;
        if(cur.dp>dp[i][j]||cur.dp==dp[i][j]&&dp_path[i][j].size()<cur.path.size()){
            dp[i][j]=cur.dp;
            dp_path[i][j]=cur.path;
        }
        if(cur.point==end){
            continue;
        }
        pair<int,int> previousNode;
        if(cur.path.size()>=2){
        previousNode=cur.path[cur.path.size()-2];
        }else{
            previousNode=make_pair(-1,-1);
        }
        for(auto &dir:directions){
        int ni=i+dir.first;
        int nj=j+dir.second;
        if(ni>=0 && ni<n && nj>=0 && nj<n&&mazegraph[ni][nj]!='#'&&findtime(cur,ni,nj)<visittime[ni][nj]
            &&(mazegraph[i][j]=='G'||previousNode!=make_pair(ni,nj))){
            int newval=0;
            vector<pair<int,int>> newpath=cur.path;
            newpath.push_back({ni,nj});
            if(findtime(cur,ni,nj)>0)
                newval=cur.dp;           
            else
            newval=cur.dp+mazevalue[ni][nj];
            
            pq.push({{ni,nj},newval,newpath});
            
        }
        }   
            
    }
}
int dpmove::findtime(Node cur,int x,int y)
{
    int count=0;
    for(auto &p:cur.path){
        if(p.first==x&&p.second==y)
        {
            count++;
        }
    }
    return count;
}
dpmove::dpmove(int n):maze(n){   }
dpmove::dpmove(string filename):maze(filename){}