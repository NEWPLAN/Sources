#include <iostream>
#include <map>
#include <string>
using namespace std;

//int pppp[1001];
int tt;

int main(void)
{
    int i=0,j=0;
    map<string,int> maps;
    string a,b;
    //maps.insert(map<string,int>::value_type("eee",1));
    map<string,int>::iterator  l_it;//=maps.find("eee");
    while((cin>>i)&&i)
    {
        int pppp[1001]= {0};
        for(int k=0; k<i; k++)
        {
            cin>>a;
            l_it=maps.find(a);
            if(l_it==maps.end())
            {
                maps.insert(map<string,int>::value_type(a,j++));
                pppp[j-1]++;
                if(pppp[tt]<pppp[j-1])
                {
                    tt=j-1;
                    b=a;
                }

            }
            else
            {
                pppp[l_it->second]++;
                if(pppp[tt]<pppp[l_it->second])
                {
                    tt=l_it->second;
                    b=a;
                }

            }

        }
        cout<<b<<endl;
        b="";
        j=0;
        tt=0;
    }
    return 0;
}
