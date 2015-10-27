#include <iostream>

#include <string>

#include <vector>

#include <fstream>

#include <cstdlib>

#include <ctime>

 

using namespace std;

 
//COUNT��MAX_NUM�ֱ��ʾ��������������ֵ

const long COUNT = 10000;

const int MAX_NUM = 200;

 
//���ļ�

bool readFile(vector<int>& input, string fileName)

{

    ifstream infile(fileName.c_str());

    if (!infile)

        return false;

    int s;

    while(infile>>s)

    {

        input.push_back(s);

    }

    return true;

}

 
//д���������������

bool writeTestData(string fileName)

{

    ofstream outfile(fileName.c_str());

    if (!outfile)

        return false;

    srand((unsigned)time(NULL));

    for (int i = 0; i < COUNT; i++)

    {

        if (rand() % 2 == 0)

            outfile << rand() % MAX_NUM << '\n';

        else

            outfile << ~(rand() % MAX_NUM) << '\n';

    }

    return true;

}

 
//��ٷ�

long maxSubSum1(const vector<int>& a)

{

    long maxSum = 0;

    for (int i = 0; i < a.size(); i++)

    {

        for (int j = i; j < a.size(); j++)

        {

            long thisSum = 0;

 

            for (int k = i; k <= j; k++)

            {

                thisSum += a[k];

            }

            if (thisSum > maxSum)

            maxSum = thisSum;

        }

    }

    return maxSum;

}

 
//Ҳ����ٷ���������ȥ�������һЩ����Ҫ����O(n^2)

long maxSubSum2(const vector<int>& a)

{

    long maxSum = 0;

    for (int i = 0; i < a.size(); i++)

    {

        long thisSum = 0;

        for (int j = i; j < a.size(); j++)

        {

            thisSum += a[j];

            if (thisSum > maxSum)

                maxSum = thisSum;

        }

    }

    return maxSum;

}

 
//�ݹ鷨�����Ӷ���O(nlogn)

long max3(long a, long b, long c)

{

    if (a < b)

    {

        a = b;

    }

    if (a > c)

        return a;

    else

    return c;

}

 
long maxSumRec(const vector<int>& a, int left, int right)

{

    if (left == right)

    {

        if (a[left] > 0)

            return a[left];

        else

            return 0;

    }

    int center = (left + right) / 2;

    long maxLeftSum = maxSumRec(a, left, center);

    long maxRightSum = maxSumRec(a, center+1, right);

 
    //�������߶Ժ�һ�����ֽ�β���������ֵ

    long maxLeftBorderSum = 0, leftBorderSum = 0;
    for (int i = center; i >= left; i--)
    {

        leftBorderSum += a[i];

        if (leftBorderSum > maxLeftBorderSum)

            maxLeftBorderSum = leftBorderSum;

    }

 
    //������ұ߶Ժ�һ�����ֽ�β���������ֵ

    long maxRightBorderSum = 0, rightBorderSum = 0;

    for (int j = center+1; j <= right; j++)

    {

        rightBorderSum += a[j];

        if (rightBorderSum > maxRightBorderSum)

            maxRightBorderSum = rightBorderSum;

    }

 

    return max3(maxLeftSum, maxRightSum,

    maxLeftBorderSum + maxRightBorderSum);

}

 
long maxSubSum3(const vector<int>& a)

{

    return maxSumRec(a, 0, a.size()-1);

}

 
//���Ե��㷨O(N)

long maxSubSum4(const vector<int>& a)

{

    long maxSum = 0, thisSum = 0;

    for (int j = 0; j < a.size(); j++)

    {

        thisSum += a[j];

        if (thisSum > maxSum)

            maxSum = thisSum;

        else if (thisSum < 0)

            thisSum = 0;

    }

    return maxSum;

}

 

int main ()

{

    vector<int> input;

    /**

    if (!writeTestData("in.txt"))

    {

        cout << "д�ļ�����" << endl;

    }

    */

 
    if (readFile(input, "in.txt"))

    {

        //cout << maxSubSum1(input) << endl;

        //cout << maxSubSum2(input) << endl;

        cout << maxSubSum3(input) << endl;

        cout << maxSubSum4(input) << endl;

    }

 

    return 0;

}