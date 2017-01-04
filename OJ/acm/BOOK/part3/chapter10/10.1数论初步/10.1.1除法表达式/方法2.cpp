#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cctype>
#include<cmath>
#include<ctime> 
const int MAXK = 10010;
int x[MAXK], k, c;
int prime[5000];//reminder: change to short in p1
bool vis[31630];
void getprime(){ //Ԥ����sqrt(1000000000)�ڵ���������Ԥ����Ҳ�ɣ���P185�� 
  int n = 31630;
  int m = (int)sqrt(n + 0.5);
  c = 0;
  memset(vis, 0, sizeof(vis));
  for(int i = 2; i <= m; ++i) if(!vis[i])
    for(int j = i*i; j <= n; j+=i) vis[j] = 1;    
  for(int i = 2; i <= n; ++i) if(!vis[i]) prime[++c] = i;
}  
int p[100], a[100];//100�������x2��������Ӧ�ù��� 
void solve(){
  //�ֽ�x2
  int sum = 0;
  for(int i = 1; i <= c; ++i) if(x[2]%prime[i]==0){
    p[++sum] = prime[i]; a[sum] = 0;
    while(x[2]%prime[i]==0) {++a[sum]; x[2]/=prime[i];}
    if(x[2] == 0) break;
  }
  x[k+1] = x[1];
  int ok = 1;
  for(int i = 1; i <= sum; ++i){//����x[1],x[3]...x[k]��p[i]��ָ��
    int found = 0, flag = 1;
    for(int j = 3; j <= k+1; ++j) if(x[j]%p[i]==0)
      while(x[j]%p[i]==0) {++found; x[j]/=p[i];}
    if(found < a[i]) {ok = 0; break;}
  }
  if(ok) printf("YES\n"); else printf("NO\n");
}  
int main(){
  freopen("in2.txt", "r", stdin);
  char c; int t = 0, n = 1;
  getprime();
  //printf("%.3lf\n", (double)clock()/CLOCKS_PER_SEC);
  while((c=getchar())!=EOF) {
      if(isdigit(c)) t = t*10 + c - '0';
      else if(c=='/') {x[n++] = t;  t = 0;}
      else if(c=='\n') {
          x[n] = t;
          k = n;
          if(x[2] == 0) printf("NO\n");
          else if(x[2] == 1) printf("YES\n");
          else solve();
          n = 1;
          t = 0;
      }
  }           
  return 0;
}
