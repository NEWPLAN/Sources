#include <iostream>
#include "ID3.h"

using namespace std;

enum outlook {SUNNY, OVERCAST, RAIN };
enum temp    {HOT,   MILD,     COOL };
enum hum     {HIGH,  NORMAL         };
enum windy   {WEAK,  STRONG         };

int samples[14][4] =
{
    {SUNNY   ,       HOT ,      HIGH  ,       WEAK  },
    {SUNNY   ,       HOT ,      HIGH  ,       STRONG},
    {OVERCAST,       HOT ,      HIGH  ,       WEAK  },
    {RAIN    ,       MILD,      HIGH  ,       WEAK  },
    {RAIN    ,       COOL,      NORMAL,       WEAK  },
    {RAIN    ,       COOL,      NORMAL,       STRONG},
    {OVERCAST,       COOL,      NORMAL,       STRONG},
    {SUNNY   ,       MILD,      HIGH  ,       WEAK  },
    {SUNNY   ,       COOL,      NORMAL,       WEAK  },
    {RAIN    ,       MILD,      NORMAL,       WEAK  },
    {SUNNY   ,       MILD,      NORMAL,       STRONG},
    {OVERCAST,       MILD,      HIGH  ,       STRONG},
    {OVERCAST,       HOT ,      NORMAL,       WEAK  },
    {RAIN    ,       MILD,      HIGH  ,       STRONG}
};

int main()
{
    ID3 Tree(4);
    Tree.PushData((int *)&samples[0], 0);
    Tree.PushData((int *)&samples[1], 0);
    Tree.PushData((int *)&samples[2], 1);
    Tree.PushData((int *)&samples[3], 1);
    Tree.PushData((int *)&samples[4], 1);
    Tree.PushData((int *)&samples[5], 0);
    Tree.PushData((int *)&samples[6], 1);
    Tree.PushData((int *)&samples[7], 0);
    Tree.PushData((int *)&samples[8], 1);
    Tree.PushData((int *)&samples[9], 1);
    Tree.PushData((int *)&samples[10], 1);
    Tree.PushData((int *)&samples[11], 1);
    Tree.PushData((int *)&samples[12], 1);
    Tree.PushData((int *)&samples[13], 0);

    Tree.Build();
    Tree.Print();
    cout << endl;
    for(int i = 0; i < 14; ++i)
        cout << "predict value :    " <<Tree.Match( (int *)&samples[i] ) << endl;
    return 0;
}
