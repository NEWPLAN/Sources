#include "AVL.h"
int arrs[7]={88,70,61,96,120,90,65};
int main(int argc,char** argv)
{
	AVLTree<int> te;
	for (int i=0;i<7;i++)
	{
		te.insert(arrs[i]);
	}
	te.Treversal();
	return 0;
}