#include <iostream>
#include <time.h>

using namespace std;
#define MAX_IN 10000
int main(void)
{
    clock_t t=clock();

    int tt=0;
    cout<<"test for here"<<endl;
    float p1=(float)t/CLOCKS_PER_SEC;
    //for(int i=0;i<10;i++)
        for(int j=0;j<MAX_IN;j++)
            for(int k=0;k<MAX_IN;k++)
                if(tt)
                    tt=!tt;
    t=clock();
    float p2=(float)t/CLOCKS_PER_SEC;
    cout<<p2-p1<<endl;
    return 0;
}
