#include <stdio.h>
#include <string.h>
#define MAXLEN 100

void LCSLength(char *x, char *y, int m, int n, int c[][MAXLEN], int b[][MAXLEN])
{
    int i, j;
    
    for(i = 0; i <= m; i++)
        c[i][0] = 0;
    for(j = 1; j <= n; j++)
        c[0][j] = 0;
    for(i = 1; i<= m; i++)
    {
        for(j = 1; j <= n; j++)
        {
            if(x[i-1] == y[j-1])
            {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 0;
            }
            else if(c[i-1][j] >= c[i][j-1])
            {
                c[i][j] = c[i-1][j];
                b[i][j] = 1;
            }
            else
            {
                c[i][j] = c[i][j-1];
                b[i][j] = -1;
            }
        }
    }
}

void PrintLCS(int b[][MAXLEN], char *x, int i, int j)
{
    if(i == 0 || j == 0)
        return;
    if(b[i][j] == 0)
    {
        PrintLCS(b, x, i-1, j-1);
        printf("%c ", x[i-1]);
    }
    else if(b[i][j] == 1)
        PrintLCS(b, x, i-1, j);
    else
        PrintLCS(b, x, i, j-1);
}

int main(int argc, char **argv)
{
    char x[MAXLEN] = {"ABCBDAB"};
    char y[MAXLEN] = {"BDCABA"};
    int b[MAXLEN][MAXLEN];
    int c[MAXLEN][MAXLEN];
    int m, n;
    
    m = strlen(x);
    n = strlen(y);
    
    LCSLength(x, y, m, n, c, b);
    PrintLCS(b, x, m, n);
    
    return 0;
}