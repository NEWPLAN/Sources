#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;
int arrs[100];
class node
{
public: 
	int data;
	node*lchild,*rchild;
	node():lchild(NULL),rchild(NULL){}

};
class BST
{
private:
	node* root;
	queue<node*> contains;
public:
	BST():root(NULL){}
	void wide_visit()
	{
		node* p=root;
		while(p)
		{
			cout<<p->data;
			if (p->lchild)
				contains.push(p->lchild);
			if (p->rchild)
				contains.push(p->rchild);
			p=contains.front();
			contains.pop();
		}
		
	}
	void insert(int*a,int n)
	{

	}
	void build_bst(int*a,int beg,int ends,node* p)
	{
		node * t=p;
	}
};
int compares(const void*a,const void*a2)
{
	return *(int*)a>*(int*)a2;
}

int main(int argc,char** argv)
{
	int n=0,j=0;
	cin>>n;
	while(j<n)
	{
		cin>>arrs[j++];
	}
	qsort(arrs,n,sizeof(int ),compares);
	return 0;
}