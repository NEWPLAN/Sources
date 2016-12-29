/*
Fibonacci Again
Problem Description
There are another kind of Fibonacci numbers: F(0) = 7, F(1) = 11, F(n) = F(n-1) + F(n-2) (n>=2).

Input
Input consists of a sequence of lines, each containing an integer n. (n < 1,000,000).

Output
Print the word "yes" if 3 divide evenly into F(n).
Print the word "no" if not.

Sample Input
0   1   2   3   4   5
Sample Output
no  no  yes no  no  no

method if mod is 3, then you can just list all scenario to find the priod current priod is 8, so you just calculate
*/

#include <stdio.h>

int main(int argc, char* argv[])
{
    int n=0;
    int a[8]={1,2,0,2,2,1,0,1};

    while(scanf("%d",&n)!=EOF)
    {
        if(a[n%8]==0)
            printf("yes\n");
        else
            printf("no\n");
    }
    return 0;
}
