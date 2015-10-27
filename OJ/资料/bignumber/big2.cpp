//���һ��֧�ִ�������ļ����������г˷�ʹ�÷��η���⡣�ü�����֧�ּӼ��˳����п��������㡣
#include <iostream>
#include <list>
#include <string>
#include <cstdio>
#include <cctype>
#include <cmath>
using namespace std;
list<char> Add(list<char> s, list<char> t);
list<char> Sub(list<char> s, list<char> t);
list<char> Mul(list<char> s, list<char> t);
void Div(list<char> s, list<char> t);
void  Root(list<char>);
void print(list<char> ans);
void printhelp()             //��ӡ������Ϣ
{
    cout << "��ѡ��Ҫ���еĴ�������" << endl;
    cout << "1:�ӷ�����" << endl;
    cout << "2:��������" << endl;
    cout << "3:�˷�����" << endl;
    cout << "4:��������" << endl;
    cout << "5:��ƽ��������" << endl;
    cout << "6:�˳�" << endl;
}
list<char> Add(list<char> num1,list<char> num2)  //�ӷ�����
{
    list<char> ans;
    list<char>::iterator iter1,iter2;
    iter1 = num1.begin();
    iter2 = num2.begin();
    int sign = 0;                                   //��ǽ������
    if((*iter1) == '-' && (*iter2) == '-')        //������������Ǹ���
    {
        num1.pop_front();
        num2.pop_front();
        sign = 1;
        ans = Add(num1,num2);
        ans.push_front('-');
    }
    else if((*iter1) == '-' && (*iter2) != '-')      //���һ��һ��
    {
        num1.pop_front();
        ans = Sub(num2,num1);

    }
    else if((*iter1) != '-' && (*iter2) == '-')      //���һ��һ��
    {
        num2.pop_front();
        ans = Sub(num1,num2);
    }
    else                                          //�����Ϊ��
    {
        int len1,len2,i,len,carry;
        len1 = num1.size();
        len2 = num2.size();
        if(len1 >= len2)                     //������������λ��
        {
            len = len1;
            for(i = 0; i < len1 - len2; i++)
                num2.push_front('0');
        }
        else
        {
            len = len2;
            for(i = 0; i < len2 - len1; i++)
                num1.push_front('0');
        }
        //print(num1);
        //print(num2);
        carry = 0;
        iter1 = num1.end();
        iter2 = num2.end();
        iter1--;
        iter2--;
        for(;(iter1 != num1.begin()) && (iter2 != num2.begin()); --iter1,--iter2)  //��������
        {
            i = (*iter1 - '0') + (*iter2 - '0') + carry;
            //cout << (*iter1 - '0') << " " << (*iter2 - '0') << " " << i << endl;
            ans.push_front((i % 10) + '0');
            carry = i / 10;
        }
        i = (*iter1 - '0') + (*iter2 - '0') + carry;
        //cout << (*iter1 - '0') << " " << (*iter2 - '0') << " " << i << endl;
        ans.push_front((i % 10) + '0');
        carry = i / 10;
        if(carry)
            ans.push_front(carry+'0');
    }
    return ans;                                                     //���ؽ��
}
list<char> Sub(list<char> num1,list<char> num2)                    //��������
{
    list<char> ans;
    int sign = 0;
    list<char>::iterator iter1,iter2;
    int len1,len2,len;
    iter1 = num1.begin();
    iter2 = num2.begin();
    if((*iter1) == '-' && (*iter2) == '-')                  //�����������Ϊ����
    {
        num2.pop_front();
        num1.pop_front();
        ans = Sub(num2,num1);
        //ans.push_front('-');
    }
    else if( (*iter1) == '-' && (*iter2) != '-')              //���һ��һ��
    {
        num1.pop_front();
        ans = Add(num1,num2);
        ans.push_front('-');
    }
    else if( (*iter1) != '-' && (*iter2) == '-')              //���һ��һ��
    {
        num2.pop_front();
        ans = Add(num1,num2);

    }
    else                                                   //�����Ϊ��
    {
        len1 = num1.size();
        len2 = num2.size();
        if(len1 >= len2)                                 //����λ��
        {
            len = len1;
            for(int i = 0; i < len1 - len2; i++)
                num2.push_front('0');
        }
        else
        {
            len = len2;
            for(int i = 0; i < len2 - len1; i++)
                num1.push_front('0');
        }
        //print(num1);cout << endl;
        //print(num2);cout << endl;
        int carry = 0,i;
        iter1 = num1.end();
        iter2 = num2.end();
        iter1--;
        iter2--;
        for(;(iter1 != num1.begin()) && (iter2 != num2.begin()); --iter1,--iter2)    //��������
        {
            i = (*iter1 - '0' - carry) - (*iter2 - '0');
            carry = 0;
            if( i < 0)
            {
                i += 10;
                carry = 1;
            }
            //cout << (*iter1 - '0') << " " << (*iter2 - '0') << " " << i << endl;
            ans.push_front((i % 10) + '0');
        }
        i = (*iter1 - '0' - carry) - (*iter2 - '0');
        if(i < 0)
        {
            i += 10;
            sign = 1;
        }
        //cout << (*iter1 - '0') << " " << (*iter2 - '0') << " " << i << endl;
        if(i) ans.push_front(i + '0');
        if(sign)
            ans.push_front('-');
    }
    return ans;
}
list<char> Mul(list<char> num1,list<char> num2)  // ���η����������Ļ�
{
    list<char> ans;
    int sign = 0;
    int len1,len2,len;
    list<char>::iterator iter1,iter2,iter;
    list<char> high,low;
    list<char> anshigh,anslow;
    int th,tl;
    int i,j,k;
    //print(num1);cout << endl;
    //print(num2);cout << endl;
    if(num1.size() == 1 && num2.size() == 1)     //�������������һλ�������������
    {
        th = *(num1.begin()) - '0';
        tl = *(num2.begin()) - '0';
        th *= tl;
        ans.push_front( th % 10 + '0');
        ans.push_front( th / 10 + '0');
        return ans;
    }
    else if(num1.size() == 1 && num2.size() > 1)            //���num1λ������1�����Num1�������
    {
         if(*(num2.begin()) == '-')
           {
                sign = 1;
                num2.pop_front();
           }
         len2 = num2.size();
         if(len2 == 1)
         {
            ans = Mul(num1,num2);
            if(sign)
                ans.push_front('-');
         }
         else
         {
            for(iter= num2.begin(),i = 0; i < len2 / 2; i++,iter++)
            {
                high.push_back(*iter);
            }
            for(;iter!=num2.end();iter++)
            {
                low.push_back(*iter);
            }
            len = low.size();
            anshigh = Mul(num1,high);                 //num1��Ϊ�����֣�high,low
            anslow = Mul(num1,low);
            for(i = 0; i < len; i++)
                anshigh.push_back('0');
            ans = Add(anshigh,anslow);                 //�ϲ����
            if(sign)
                ans.push_front('-');
         }
         return ans;
    }
    else if(num2.size() == 1 && num1.size() > 1)              //���num2λ������1�����Num2�������
    {
         if(*(num1.begin()) == '-')
           {
                sign = 1;
                num1.pop_front();
           }
         len1 = num1.size();
         if(len2 == 1)
         {
            ans = Mul(num1,num2);
            if(sign)
                ans.push_front('-');
         }
         else
         {
            for(iter= num1.begin(),i = 0; i < len1 / 2; i++,iter++)
            {
                high.push_back(*iter);
            }
            for(;iter!=num1.end();iter++)
            {
                low.push_back(*iter);
            }
            len = low.size();
            anshigh = Mul(num2,high);                   //num2��Ϊ�����֣�high,low
            anslow = Mul(num2,low);
            for(i = 0; i < len; i++)
                anshigh.push_back('0');
            ans = Add(anshigh,anslow);                    //�ϲ����
            if(sign)
                ans.push_front('-');
         }
         return ans;
    }                                                       //�������λ��������1�������÷���
    else
    {
        list<char> num1high,num1low,num2high,num2low;
        int flag1 = 0,flag2 = 0;
        if(*(num1.begin()) == '-')
        {
            flag1 = 1;
            num1.pop_front();
        }
        if(*(num2.begin()) == '-')
        {
            flag2 = 1;
            num2.pop_front();
        }
        if((flag1 == 1 && flag2 == 0)||(flag1 == 0 && flag2 == 1))  //�����һ��һ�������ǽ��Ϊ��
        {
            sign = 1;
        }
        len1 = num1.size();
        len2 = num2.size();
        if(len1 == 1 || len2 == 1)                 //�����һ������һλ������ֱ�ӵݹ����
        {
            ans = Mul(num1,num2);
            if(sign)
                ans.push_front('-');
        }
        else
        {                                                //������η���
            for(iter = num1.begin(),i = 0; i < len1 / 2; iter++,i++)
                num1high.push_back(*iter);            //��������λ����
            for( ; iter != num1.end(); iter++)
                num1low.push_back(*iter);                 //��������λ����
            for(iter = num2.begin(),i = 0; i < len2 / 2; iter++,i++)
                num2high.push_back(*iter);                  //������λ����
            for( ; iter != num2.end(); iter++)
                num2low.push_back(*iter);                    //������λ����
            int a = (len1 + 1) / 2;
            int b = (len2 + 1) / 2;
            list<char> AC,AD,BC,BD;
            //print(num2high);cout << endl;
            //print(num2low);cout << endl;
            AC = Mul(num1high,num2high);                  //����X=A*10^a + B; Y= C*10^b + D;
            AD = Mul(num1high,num2low);                   // X*Y = AC * 10 ^(a+b) + AD *10^a + BC * 10 ^b + BD��ʽ��
            BC = Mul(num1low,num2high);
            BD = Mul(num1low,num2low);
            for(i = 0; i < a + b; i++)
                AC.push_back('0');
            for(i = 0; i < a; i++)
                AD.push_back('0');
            for(i = 0; i < b; i++)
                BC.push_back('0');
            ans = Add(AC,AD);
            ans = Add(ans,BC);
            ans = Add(ans,BD);                            //�ۼӽ��
            if(sign)
                ans.push_front('-');
        }
        return ans;
    }
}
void Div(list<char> num1,list<char> num2)                 //��շת�������
{
    list<char> ans;
    list<char> temp;
    int len1,len2,len;
    int i,j,k;
    int sign = 0;
    int flag1 = 0,flag2 = 0;
    list<char>::iterator iter;
    if(*(num1.begin()) == '-')
    {
        flag1 = 1;
        num1.pop_front();
    }
    if(*(num2.begin()) == '-')
    {
        flag2 = 1;
        num2.pop_front();
    }
    if((flag1 == 1 && flag2 != 1) || (flag1 == 0 && flag2 == 1))
        sign = 1;                                          //��ǽ������
    len1 = num1.size();
    len2 = num2.size();
    if(len1 < len2)                                 //���������С�ڳ���������Ϊ0
    {
        cout << "����0��������" ;
        print(num2);
        cout << endl;
        return ;
    }
    else                                               //��շת�������
    {
        for(iter = num1.begin(),i = 0; i < len2 ;++i,++iter)
            temp.push_back(*iter);
        list<char> digit,res;
        for(j = 0; j < 10; j++)
        {
                digit.clear();
                digit.push_back(j + '0');
                res = Mul(num2,digit);
                res = Sub(temp,res);
                if(*(res.begin()) == '-')
                    break;
        }
            //cout << endl;print(temp); cout << endl;
        j -= 1;
        if(j > 0)
        {
            digit.clear();
            digit.push_back(j + '0');
            temp = Sub(temp,num2);
            ans.push_back(j + '0');
        }

        for(;iter != num1.end();++iter)
        {
            temp.push_back(*iter);
            for(j = 0; j < 10; j++)
            {
                digit.clear();
                digit.push_back(j + '0');
                res = Mul(num2,digit);
                res = Sub(temp,res);
                if(*(res.begin()) == '-')
                    break;
            }
            //cout << endl;print(temp); cout << endl;
            j -= 1;
            digit.clear();
            digit.push_back(j + '0');
            res = Mul(num2,digit);
            temp = Sub(temp,res);
            ans.push_back(j + '0');
        }
        if(sign)
            ans.push_front('-');
    }
    cout << "����";
    print(ans);
    cout << "��������" ;
    print(temp);
    cout << endl;
    return ;
}
void  Root(list<char> num1)                     //�������ƽ��������ö���㷨���������㷨��
{
    list<char> ans;
    int len1;
    list<char> digit,temp;
    len1 = num1.size();
    int a = len1 / 2;                            //a�ǽ��������λ��
    int i;
    digit.push_back('1');
    ans.push_back('1');                        //��ans=10 ^ a ��ʼ����
    for(i = 0; i < a - 1; i++)
        ans.push_back('0');
    while(true)                                  //�ѵ������
    {
        temp = Mul(ans,ans);
        temp = Sub(num1,temp);
        if(*(temp.begin()) == '-')                      //�����ǰans��ƽ������num1����ý��
            break;
        ans = Add(ans,digit);
    }
    ans = Sub(ans,digit);
    print(ans);
    cout << endl;
}
list<char> translate(string input)
{
    list<char> res;
    int len = input.length();
    for(int i = 0; i < len; i++)
        res.push_back(input[i]);
    return res;
}
void print(list<char> ans)
{
    list<char>::iterator iter;
    int flag = 1;
    iter = ans.begin();
    if(*iter == '-')
     {
        cout << *iter;
        iter++;
     }
    for(;iter != ans.end(); ++iter)
    {
        if(*iter == '0' && flag)
            continue;
            //cout << (*iter);
        else
        {
            flag = 0;
            cout << (*iter);
        }
    }
    if( flag == 1)
        cout << 0 ;;
    //cout << endl;
}
int main()
{
    char choice;         //ѡ��Ҫ���е�����
    list<char> num1;
    list<char> num2;
    list<char> res;
    string input;
    while(true)
    {
        cout << endl;
        printhelp();
        cin >> choice;
        switch(choice)
        {
            case '1':                      //���ѡ��ӷ�����
                cout << "�������һ����" << endl;
                cin >> input;
                num1 = translate(input);
                cout << "������ڶ�����" << endl;
                cin >> input;
                num2 = translate(input);
                res = Add(num1,num2);
                print(num1);
                cout << " + " ;
                print(num2);
                cout << " = ";
                print(res);
                cout << endl;
                break;
            case '2':                    //ѡ���������
                cout << "�������һ����" << endl;
                cin >> input;
                num1 = translate(input);
                cout << "������ڶ�����" << endl;
                cin >> input;
                num2 = translate(input);
                res = Sub(num1,num2);
                print(num1);
                cout <<" - " ;
                print(num2);
                cout << " = ";
                print(res);
                cout << endl;
                break;
            case '3':                  //ѡ��˷�����
                cout << "�������һ����" << endl;
                cin >> input;
                num1 = translate(input);
                cout << "������ڶ�����" << endl;
                cin >> input;
                num2 = translate(input);
                res = Mul(num1,num2);
                print(num1);
                cout <<" * " ;
                print(num2);
                cout << " = ";
                print(res);
                cout << endl;
                break;
            case '4':                      //ѡ���������
                cout << "�������һ����" << endl;
                cin >> input;
                num1 = translate(input);
                cout << "������ڶ�����" << endl;
                cin >> input;
                num2 = translate(input);
                print(num1);
                cout << " / " ;
                print(num2);
                cout << " = ";
                if(num2.size() == 1 && (*(num2.begin()) == '0'))
                {
                    cout << "divided zero error" << endl;
                    break;
                }
                Div(num1,num2);
                //print(res);
                cout << endl;
                break;
            case '5':                         //ѡ���������
                cout << "�������һ����" << endl;
                cin >> input;
                num1 = translate(input);
                if(*(num1.begin()) == '-')
                {
                    cout << "����û��ʵ��ƽ����" << endl;
                    break;
                }
                Root(num1);
                break;
            case '6':
                return 0;
            default:
                  break;
        }
    }
}
