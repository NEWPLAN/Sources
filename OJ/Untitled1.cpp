/*
 *      �������ת�� 72,75��������һ����ֱ��������ʱ��ֵ��һ����֮��ֵ
 *      ��� L,R��ʾ���캯�� Lc,Rc��ʾ���ƹ���  ~R,~L��ʾ����
 *      n��������������Ķ�ÿ������ı�־��numΪ�ڴ�������
 */
#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;
static int num=0;
class L;
class R
{
public:
    R(int _w=1,int _h=3):w(_w),h(_h)
    {
        n=rand();
        num++,cout<<"R,num="<<num<<",n="<<n<<endl;
    }
    ~R()
    {
        num--,cout<<"~R,num="<<num<<",n="<<n<<endl;
    }
    R(const R& r)
    {
        n=rand();
        num++,cout<<"Rc,num="<<num<<",n="<<n<<endl;
    }
    friend class L;
    operator L();
    friend ostream& operator<<(ostream& os,const R& r)
    {
        os <<r.w<<','<<r.h;
        return os;
    }
    /*
    R& operator=(R& r)
    {
        w=r.w;
        h=r.h;
        cout<<"operator="<<endl;
        return *this;
    }
    */
private:
    int w,h;
    long n;
};
class L
{
public:
    L(int _w=1,int _h=3):w(_w),h(_h)
    {
        n=rand();
        num++,cout<<"L,num="<<num<<",n="<<n<<endl;
    }
    ~L()
    {
        num--,cout<<"~L,num="<<num<<",n="<<n<<endl;
    }
    L(const L& l)
    {
        n=rand();
        num++,cout<<"Lc,num="<<num<<",n="<<n<<endl;
    }
    friend class R;
    operator R();
    friend ostream& operator<<(ostream& os,const L& r)
    {
        os <<r.w<<','<<r.h;
        return os;
    }
private:
    int w,h;
    long n;
};
L::operator R()
{
    R r;
    r.w=h;
    r.h=w;
    return r;
}
R::operator L()
{
    L l;
    l.w=h;
    l.h=w;
    return l;
}
int main()
{
    srand(static_cast<unsigned>(time(NULL)));
    R r1,r2=r1,r3(r1);
    L l=r1;
    cout<<l<<endl;
    R x;
    x=R(l);
    cout<<x<<endl;
    return 0;
}
