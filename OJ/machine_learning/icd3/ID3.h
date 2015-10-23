#ifndef _ID3_H_
#define _ID3_H_

#include <utility>
#include <list>
#include <map>

#define Type int   //������������

#define   Map1        std::map< int, Type >    //����һάmap
#define   Map2        std::map< int, Map1 >    //�����άmap
#define   Map3        std::map< int, Map2 >    //������άmap
#define   Pair        std::pair<int, Type>
#define   List        std::list< Pair >        //һάlist
#define   SampleSpace std::list< List >        //��άlist ���ڴ����������
#define   Child       std::map< int, Node* >   //�����̽ڵ㼯��
#define   CI          const_iterator

/*
 *   ��ID3�㷨�У��ö�ά�������������ṹΪlist< list< pair<int, int> > >�����ΪSampleSpace��ȡ�������ռ�
 *   �������ݴӸ��ڵ㿪ʼ���±�����ÿһ���ڵ�Ķ������½ṹ��
 */

struct Node
{
    int index;                    //��ǰ�ڵ�������������Ӧ��index�����ԣ�����������з����
    int type;                     //��ǰ�ڵ������
    Child next;                   //��ǰ�ڵ�ĺ�̽ڵ㼯��
    SampleSpace sample;           //δ�������������
};

class ID3
{

public:

    ID3(int );
    ~ID3();

    void PushData(const Type*, const Type);   //����������Push����ά����
    void Build();                             //����������
    int  Match(const Type*);                  //�����µ�����Ԥ����
    void Print();                             //��ӡ�������Ľڵ��ֵ

private:

    void   _clear(Node*);
    void   _build(Node*, int);
    int    _match(const int*, Node*);
    void   _work(Node*);
    double _entropy(const Map1&, double);
    int    _get_max_gain(const SampleSpace&);
    void   _split(Node*, int);
    void   _get_data(const SampleSpace&, Map1&, Map2&, Map3&);
    double _info_gain(Map1&, Map2&, double, double);
    int    _same_class(const SampleSpace&);
    void   _print(Node*);

private:

    int dimension;
    Node *root;
};

#endif // _ID3_H_
