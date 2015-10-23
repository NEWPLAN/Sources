#include <iostream>
#include <cassert>
#include <cmath>

#include "ID3.h"

using namespace std;

//��ʼ��ID3�����ݳ�Ա
ID3::ID3(int dimension)
{
    this->dimension = dimension;

    root = new Node();
    root->index = -1;
    root->type = -1;
    root->next.clear();
    root->sample.clear();
}

//�������������
ID3::~ID3()
{
    this->dimension = 0;
    _clear(root);
}

//xΪdimensionά������������yΪ����x��Ӧ��ֵ
void ID3::PushData(const Type *x, const Type y)
{
    List single;
    single.clear();
    for(int i = 0; i < dimension; i++)
        single.push_back(make_pair(i + 1, x[i]));
    single.push_back(make_pair(0, y));
    root->sample.push_back(single);
}

void ID3::_clear(Node *node)
{
    Child &next = node->next;
    Child::iterator it;
    for(it = next.begin(); it != next.end(); it++)
        _clear(it->second);
    next.clear();
    delete node;
}

void ID3::Build()
{
    _build(root, dimension);
}

void ID3::_build(Node *node, int dimension)
{
    //��ȡ��ǰ�ڵ�δ�������������
    SampleSpace &sample = node->sample;

    //�жϵ�ǰ���������Ƿ���ͬһ�࣬��������򷵻�-1
    int y = _same_class(sample);

    //�����������������ͬһ��
    if(y >= 0)
    {
        node->index = -1;
        node->type = y;
        return;
    }

    //��_max_gain()�����м������ǰ�ڵ����������Ӧ�����ԣ�������������Զ����ݽ��л���
    _work(node);

    //Split��ɺ���յ�ǰ�ڵ���������ݣ�����ռ��̫���ڴ�
    sample.clear();

    Child &next = node->next;
    for(Child::iterator it = next.begin(); it != next.end(); it++)
        _build(it->second, dimension - 1);
}

//�жϵ�ǰ���������Ƿ���ͬһ�࣬��������򷵻�-1
int ID3::_same_class(const SampleSpace &ss)
{
    //ȡ����ǰ�������ݵ�һ��Sample
    const List &f = ss.front();

    //���û��x���ԣ���ֻ��y��ֱ�ӷ���y
    if(f.size() == 1)
        return f.front().second;

    Type y = 0;
    //ȡ����һ����������y�Ľ��ֵ
    for(List::CI it = f.begin(); it != f.end(); it++)
    {
        if(!it->first)
        {
            y = it->second;
            break;
        }
    }

    //�����������жϣ���Ϊlist������ģ����Դ�ǰ���������������һ�Բ�һ������������������ͬһ��
    for(SampleSpace::CI it = ss.begin(); it != ss.end(); it++)
    {
        const List &single = *it;
        for(List::CI i = single.begin(); i != single.end(); i++)
        {
            if(!i->first)
            {
                if(y != i->second)
                    return -1;         //���ֲ���ͬһ���򷵻�-1
                else
                    break;
            }
        }
    }
    return y;     //�Ƚ����������������ֵy�󣬷�����ͬһ�࣬����yֵ��
}

void ID3::_work(Node *node)
{
    int mai = _get_max_gain(node->sample);
    assert(mai >= 0);
    node->index = mai;
    _split(node, mai);
}

//��ȡ������Ϣ�����Ӧ������
int ID3::_get_max_gain(const SampleSpace &ss)
{
    Map1 y;
    Map2 x;
    Map3 xy;

    _get_data(ss, y, x, xy);
    double s = ss.size();
    double entropy = _entropy(y, s);   //������ֵ

    int mai = -1;
    double mag = -1;

    for(Map2::iterator it = x.begin(); it != x.end(); it++)
    {
        double g = _info_gain(it->second, xy[it->first], s, entropy);    //������Ϣ����ֵ
        if(g > mag)
        {
            mag = g;
            mai = it->first;
        }
    }

    if(!x.size() && !xy.size() && y.size())   //���ֻ��y����
        return 0;
    return mai;
}

//��ȡ���ݣ���ȡ������������yֵ��x[]����ֵ���Լ�����ֵ�ͽ��ֵxy��
void ID3::_get_data(const SampleSpace &ss, Map1 &y, Map2 &x, Map3 &xy)
{
    for(SampleSpace::CI it = ss.begin(); it != ss.end(); it++)
    {
        int c = 0;
        const List &v = *it;
        for(List::CI p = v.begin(); p != v.end(); p++)
        {
            if(!p->first)
            {
                c = p->second;
                break;
            }
        }
        ++y[c];
        for(List::CI p = v.begin(); p != v.end(); p++)
        {
            if(p->first)
            {
                ++x[p->first][p->second];
                ++xy[p->first][p->second][c];
            }
        }
    }
}

//������ֵ
double ID3::_entropy(const Map1 &x, double s)
{
    double ans = 0;
    for(Map1::CI it = x.begin(); it != x.end(); it++)
    {
        double t = it->second / s;
        ans += t * log2(t);
    }
    return -ans;
}

//������Ϣ����
double ID3::_info_gain(Map1 &att_val, Map2 &val_cls, double s, double entropy)
{
    double gain = entropy;
    for(Map1::CI it = att_val.begin(); it != att_val.end(); it++)
    {
        double r = it->second / s;
        double e = _entropy(val_cls[it->first], it->second);
        gain -= r * e;
    }
    return gain;
}

//�Ե�ǰ�ڵ��sample���л���
void ID3::_split(Node *node, int idx)
{
    Child &next = node->next;
    SampleSpace &sample = node->sample;

    for(SampleSpace::iterator it = sample.begin(); it != sample.end(); it++)
    {
        List &v = *it;
        for(List::iterator p = v.begin(); p != v.end(); p++)
        {
            if(p->first == idx)
            {
                Node *tmp = next[p->second];
                if(!tmp)
                {
                    tmp = new Node();
                    tmp->index = -1;
                    tmp->type = -1;
                    next[p->second] = tmp;
                }
                v.erase(p);
                tmp->sample.push_back(v);
                break;
            }
        }
    }
}

int ID3::Match(const Type *x)
{
    return _match(x, root);
}

int ID3::_match(const Type *v, Node *node)
{
    if(node->index < 0)
        return node->type;

    Child &next = node->next;
    Child::iterator p = next.find(v[node->index - 1]);
    if(p == next.end())
        return -1;

    return _match(v, p->second);
}

void ID3::Print()
{
    _print(root);
}

void ID3::_print(Node *node)
{
    cout << "Index    = " << node->index << endl;
    cout << "Type     = " << node->type << endl;
    cout << "NextSize = " << node->next.size() << endl;
    cout << endl;

    Child &next = node->next;
    Child::iterator p;
    for(p = next.begin(); p != next.end(); ++p)
        _print(p->second);
}
