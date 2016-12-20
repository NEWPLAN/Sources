
#include "data_structure.h"
#include<iostream>
using namespace std;

//segment_tree
extern seg_node* segment_tree_build(int beg, int end);
extern void segment_tree_insert(seg_node *root, int beg, int end);
extern int segment_tree_length(seg_node *root);
extern void segment_tree_print(seg_node *root);//for test

int main()
{
	//segment tree
	cout << endl << "segment tree [0, 10]" << endl;
	seg_node *root = segment_tree_build(0, 10);
	segment_tree_insert(root, 2, 4);
	segment_tree_insert(root, 6, 9);
	cout << "insert [2, 4], [6, 9], length: " << segment_tree_length(root) << endl;
	segment_tree_print(root);
	segment_tree_insert(root, 5, 7);
	cout << "insert [5, 7], length: " << segment_tree_length(root) << endl;
	segment_tree_print(root);
	
	return(0);
}