#include "maze.h"
bool maze::has_unique_path() {
    visited.assign(n, vector<bool>(n, false));
    int path_count = 0;
    dfs(start.first, start.second, path_count);
    return (path_count == 1);
}

bool maze::dfs(int x, int y, int& path_count) {
    if (x == end.first && y == end.second) {
        path_count++;
        return true;
    }
    if (visited[x][y]) {
        return false;
    }
    visited[x][y] = true; 
    for (const auto& dir : directions) {
        if(x+dir.first>=0 && x+dir.first<n && y+dir.second>=0 && y+dir.second<n && mazegraph[x+dir.first][y+dir.second]!='#'){
            int nx = x + dir.first;
            int ny = y + dir.second;
            dfs(nx, ny, path_count);
        }
    }
    visited[x][y] = false;  
    return (path_count > 0); 
}
maze::maze(string filename){
    readjson(filename);
    n=mazegraph.size();
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(mazegraph[i][j]=='S'){
                start.first=i;
                start.second=j;
            }
            if(mazegraph[i][j]=='E'){
                end.first=i;
                end.second=j;
            }
            if(mazegraph[i][j]=='G'){
                resourse++;
            }
            if(mazegraph[i][j]=='L'){
                locker++;
            }
            if(mazegraph[i][j]=='T'){
                trap++;
            }
        }
    }
    printmaze();
    initvalue();
}
void maze::readjson(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("无法打开文件: " + filename);
    }

    try {
        nlohmann::json jsonData;
        file >> jsonData;
        
        // 清空现有数据
        mazegraph.clear();
        
        // 检查是否是二维数组
        if (!jsonData.is_array()) {
            throw runtime_error("JSON 数据不是数组格式");
        }

        // 遍历每一行
        for (const auto& row : jsonData) {
            if (!row.is_array()) {
                throw runtime_error("JSON 行数据不是数组");
            }

            vector<char> currentRow;
            // 遍历每个字符元素
            for (const auto& element : row) {
                // 确保元素是字符串且长度为1
                if (!element.is_string() || element.get<string>().size() != 1) {
                    throw runtime_error("JSON 元素不是单字符字符串");
                }
                currentRow.push_back(element.get<string>()[0]);
            }
            mazegraph.push_back(currentRow);
        }
        
        file.close();
    } catch (const nlohmann::json::exception& e) {
        throw runtime_error("JSON 解析错误: " + string(e.what()));
    }
}
void maze::saveVectorToJson(string filename) {
    ofstream outFile(filename);
    if (!outFile) throw runtime_error("无法打开文件");
    
    outFile << "[\n";  // 开始数组
    for (size_t i = 0; i < mazegraph.size(); ++i) {
        outFile << "    [";  // 4空格缩进
        for (size_t j = 0; j < mazegraph[i].size(); ++j) {
            outFile << "\"" << mazegraph[i][j] << "\"";
            if (j < mazegraph[i].size() - 1) outFile << ", ";
        }
        outFile << "]";
        if (i < mazegraph.size() - 1) outFile << ",";
        outFile << "\n";
    }
    outFile << "]\n";
    outFile.close();
}
int maze::getValue(int x, int y){
    switch(mazegraph[x][y]) {
        case 'G': return 5;    
        case 'T': return -3;   
        case ' ': return 0;    
        case 'S': return 0;    
        case 'E': return 0; 
        case 'L': return 0;  
        case 'B': return 0; 
        case  '#': return -10000;
        default: return -100000;
    }
    
}
void maze::initvalue()
{
    mazevalue.resize(n,vector<int>(n,-10000));
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            mazevalue[i][j]=getValue(i,j);
}
void maze::add_spe_ele(int ele_num,int ele_type)
{
    while (ele_num>0)
    {
        int i=random_int(0,n-1);
        int j=random_int(0,n-1);
        if(mazegraph[i][j]==' ')
        {
            switch (ele_type)
            {
                case 1:
                mazegraph[i][j]='T';
                break;
                case 2:
                mazegraph[i][j]='G';
                break;
                case 3:
                mazegraph[i][j]='L';
                break;
            }
            ele_num--;
        }
    }
}
int maze::random_int(int i, int j)
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(i, j);
    return dis(gen);
}
void maze::mazeinit(int x1,int x2,int y1,int y2){
   if(x2==x1||y2==y1){
    return;
   }
   int x,y;
   int count=0;
   do{
    if(count>100) break;
    x=random_int(x1,x2);
    y=random_int(y1,y2);
    count++;
    }while
    ((y1>0&&mazegraph[x][y1-1]==' ')||(y2<n-1&&mazegraph[x][y2+1]==' ')||(x1>0&&mazegraph[x1-1][y]==' ')||(x2<n-1&&mazegraph[x2+1][y]==' '));
    for(int i=y1;i<=y2;i++)
    mazegraph[x][i]='#';
    for(int i=x1;i<=x2;i++)
    mazegraph[i][y]='#';
   vector<pair<int,int>> randpoints;
   if(x!=x1&&x!=x2&&y!=y1&&y!=y2)
   {
        int count=0;
        randpoints.push_back(make_pair(x,random_int(y1,y-1)));
        randpoints.push_back(make_pair(x,random_int(y+1,y2)));
        randpoints.push_back(make_pair(random_int(x1,x-1),y));
        randpoints.push_back(make_pair(random_int(x+1,x2),y));
        bool visited[randpoints.size()]{false};
        while(count<3){
            int random_index = random_int(0, randpoints.size()-1);
            if(visited[random_index]==false)
            {
                mazegraph[randpoints[random_index].first][randpoints[random_index].second]=' ';
                visited[random_index]=true;
                count++;
            }
        }
        //printmaze();
        mazeinit(x1,x-1,y1,y-1);
        mazeinit(x+1,x2,y+1,y2);
        mazeinit(x1,x-1,y+1,y2);
        mazeinit(x+1,x2,y1,y-1);
    }
   else if((y==y1||y==y2)&&x!=x1&&x!=x2)
   {
        if(y==y1){
            for(int i=x1;i<=x2;i++){
                if(y1>0&&mazegraph[i][y1-1]==' ')
                mazegraph[i][y1]=' ';
            }
            mazegraph[x][random_int(y1+1,y2)]=' ';
            //printmaze();
            mazeinit(x1,x-1,y+1,y2);
            mazeinit(x+1,x2,y+1,y2);
        }
        else{
            for(int i=x1;i<=x2;i++){
                if(y2<n-1&&mazegraph[i][y2+1]==' ')
                mazegraph[i][y2]=' ';
            }
            mazegraph[x][random_int(y1,y2-1)]=' ';
            //printmaze();
            mazeinit(x1,x-1,y1,y-1);
            mazeinit(x+1,x2,y1,y-1);
        }
   }else if((x==x1||x==x2)&&y!=y1&&y!=y2)
   {
        if(x==x1){
            for(int i=y1;i<=y2;i++){
                if(x1>0&&mazegraph[x1-1][i]==' ')
                mazegraph[x1][i]=' ';
            }
            mazegraph[random_int(x1+1,x2)][y]=' ';
            //printmaze();
            mazeinit(x+1,x2,y1,y-1);
            mazeinit(x+1,x2,y+1,y2);
        }
        else{
            for(int i=y1;i<=y2;i++){
                if(x2<n-1&&mazegraph[x2+1][i]==' ')
                mazegraph[x2][i]=' ';
            }
            mazegraph[random_int(x1,x2-1)][y]=' ';
            //printmaze();
            mazeinit(x1,x-1,y1,y-1);
            mazeinit(x1,x-1,y+1,y2);
        }
   }else  if(x==x1&&y==y1){
        for(int i=y1;i<=y2;i++){
            if(x1>0&&mazegraph[x1-1][i]==' ')
            mazegraph[x1][i]=' ';
        }
        for(int i=x1;i<=x2;i++){
            if(y1>0&&mazegraph[i][y1-1]==' ')
            mazegraph[i][y1]=' ';
        }
        //printmaze();
        mazeinit(x1+1,x2,y1+1,y2); 
   }
   else if(x==x1&&y==y2){
        for(int i=y1;i<=y2;i++){
            if(x1>0&&mazegraph[x1-1][i]==' ')
            mazegraph[x1][i]=' ';
        }
        for(int i=x1;i<=x2;i++){
            if(y2<n-1&&mazegraph[i][y2+1]==' ')
            mazegraph[i][y2]=' ';
        }
        //printmaze();
        mazeinit(x1+1,x2,y1,y-1);
   }
   else if(x==x2&&y==y1){
        for(int i=y1;i<=y2;i++){
            if(x2<n-1&&mazegraph[x2+1][i]==' ')
                mazegraph[x2][i]=' ';
        }
        for(int i=x1;i<=x2;i++){
            if(y1>0&&mazegraph[i][y1-1]==' ')
            mazegraph[i][y1]=' ';
        }
        //printmaze();
        mazeinit(x1,x2-1,y1+1,y2);
   }
   
   else if(x==x2&&y==y2){
        for(int i=y1;i<=y2;i++){
            if(x2<n-1&&mazegraph[x2+1][i]==' ')
                mazegraph[x2][i]=' ';
        }
        for(int i=x1;i<=x2;i++){
            if(y2<n-1&&mazegraph[i][y2+1]==' ')
            mazegraph[i][y2]=' ';
        }
        //printmaze();
        mazeinit(x1,x2-1,y1,y2-1);
   }
   
   
}
bool maze::detect_isolated_point(){
    queue<pair<int,int>> q;
    visited.clear();
    visited.resize(n,vector<bool>(n,false));
    q.push(start);
    bfs(start.first,start.second,q);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(!visited[i][j]&&mazegraph[i][j]!='#')
            return true;
        }
    }
    return false;

}
void maze::bfs(int x,int y,queue<pair<int,int>> &q){
    visited[x][y]=true;
    while(!q.empty()){
        pair<int,int> cur=q.front(); 
        q.pop();
        for(auto &dir:directions)
        {
            int ni=cur.first+dir.first;
            int nj=cur.second+dir.second;
            if(ni>=0&&nj>=0&&ni<n&&nj<n&&mazegraph[ni][nj]!='#'&&!visited[ni][nj])
            {
                visited[ni][nj]=true;
                q.push(make_pair(ni,nj));
            }
        }
    }
}
maze::maze(int n)
{
    this->n=n;
    do{
        mazegraph.clear();
        mazegraph.resize(n,vector<char>(n,' '));
        mazeinit(0,n-1,0,n-1);
        addelement();
    }while(detect_isolated_point()&&!has_unique_path());
    initvalue();
    printmaze();
    saveVectorToJson("mazegraph2.json");
}
void maze::printmaze(){
    for(int i=0;i<mazegraph.size();i++){
        for(int j=0;j<mazegraph[i].size();j++){
            if(printmaze_visited[i][j]==false||mazegraph[i][j]=='M')
            cout<<mazegraph[i][j]<<" ";
            else cout<<"  ";
        }
        cout<<endl;
    }
    cout<<endl;
}
void maze::addelement(){
    trap=n/4;
    resourse=n/3;
    locker=n/5;
    int i,j;
    while(1){ //寻找起点位置
        i=random_int(0,n/4);
        j=random_int(0,n/4);
        if(mazegraph[i][j]==' ')          
        {
            mazegraph[i][j]='S';
            start=make_pair(i,j);
            break;
        }
    }
    while(1){ //寻找终点位置
        i=random_int(n/4*3,n-1);
        j=random_int(n/4*3,n-1);
        if(mazegraph[i][j]==' ')
        {
            mazegraph[i][j]='E';
            end=make_pair(i,j);
            break;
        }
    }
    while(1){ //寻找boss位置
        i=random_int(n/2,n-1);
        j=random_int(n/2,n-1);
        if(mazegraph[i][j]==' ')
        {
            mazegraph[i][j]='B';
            break;
        }
    }
    add_spe_ele(trap,1);//陷阱
    add_spe_ele(resourse,2);//资源点
    add_spe_ele(locker,3);//机关
}