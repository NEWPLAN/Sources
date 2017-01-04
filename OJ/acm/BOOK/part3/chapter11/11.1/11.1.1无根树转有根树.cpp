#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector> 
#include<queue>
using namespace std;
const int maxn = 1000;
//�����޸����Ľ�����n������n-1����(u, v)��������ָ���ĸ��ı��root��������rootΪ������
vector<int> G[maxn];
int n, root, p[maxn];
void dfs(int u, int fa){
    int d = G[u].size();
    for(int i = 0; i < d; i++){
        const int& v = G[u][i];
        if(v != fa) dfs(v, p[v] = u);
    }
}            
int main(){
    freopen("11.1.1.in", "r", stdin);
    scanf("%d", &n);
    int u, v;
    for(int i = 0; i < n-1; ++i){
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
        G[v].push_back(u);
    }
    scanf("%d", &root);
    p[root] = -1;
    dfs(root, -1);
    for(int i = 0; i < n; ++i) printf("p[%d] = %d\n", i, p[i]);
    return 0;
}    
        
