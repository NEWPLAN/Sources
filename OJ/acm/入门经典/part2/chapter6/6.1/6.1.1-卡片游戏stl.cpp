#include<iostream>
#include<cstdio>
#include<queue>
//��ʵ֤�������ֹ�ģ����е������ٶ�Զ����stl����
using namespace std;
queue<int> q;
int main(){
    int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++) q.push(i);
    while(!q.empty()){
        printf("%d ", q.front());
        q.pop();
        q.push(q.front());
        q.pop();
    }
    printf("\n");
    return 0;
}
