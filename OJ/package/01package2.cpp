#include<iostream>
# include<cstring>
# define max(a,b) a>b?a:b
using namespace std;
int main()
{

    int dp[101][1001],m,T,w[101],val[101],i,j;
    cin>>T>>m;
    for(i=1; i<=m; i++)
        cin>>w[i]>>val[i];
    memset(dp,0,sizeof(dp));
    for(i=1; i<=m; i++)
        for(j=0; j<=T; j++) //j相当于上面说的V-c[i]
        {
            if(j>=w[i])
                dp[i][j]=max(dp[i-1][j],dp[i-1][j-w[i]]+val[i]);//放还是不放的选择
            else dp[i][j]=dp[i-1][j];
        }
    cout<<dp[m][T]<<endl;
    return 0;
}
