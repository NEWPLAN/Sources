#include <iostream>

using namespace std;
struct Axis
{
    int x,y;
}boom[4];

int main(void)
{
    int R;
    cin>>R;
    int dx,dy;
    int cc=0;
    for(int i=0;i<4;i++)
        cin>>boom[i].x>>boom[i].y;
    for(int i=0;i<3;i++)
    {
        dx=boom[i].x-boom[3].x;
        dy=boom[i].y-boom[3].y;
        if(dx*dx+dy*dy<=R*R)
            cc++;
    }
    cout<<cc<<"X"<<endl;
    return 0;
}
