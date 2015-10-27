//�߶���
//segment_tree.cpp

//x������һ��ֱ�ߣ���ζ�x���ϵ�һЩ�߶ν���Ⱦɫ��ͬһ��Χ���߶ο����ظ�Ⱦɫ
//��Ⱦɫ���߶εĳ���֮��

//��ԭʼ�ķ�����������������ʾx����ֱ��
//ÿȾɫһ���߶ζ������������Ӧ������λ�ã�����ۼ�����Ⱦɫ����ĳ���֮��
//�÷�����Ȼ̫���ˣ��޷����ٵĴ��������Ⱦɫ
//
//�߶�����һ����������ÿ���ڵ㶼��һ���߶ε�����ұ�����[beg, end]
//�߶���������������һ�֣�ÿ���ڵ㶼��һ������
//�߶������ڽ�����������串�����⣬�߶�Ⱦɫ�������߶����ĵ���Ӧ�ó���
//
//�߶�����Ԫ�߶κͷ�Ԫ�߶����ֽڵ�
//Ԫ�߶��ǳ���ֻ��1���߶Σ�����[1, 2]
//��Ԫ�߶γ��ȴ���1���Ҹýڵ�[beg, end]�����������ӽڵ�
//���������߶�Ϊ[beg, (beg+end)/2]���Һ����߶�Ϊ[(beg+end)/2, end]
//�����߶�����ÿ���ڵ㣬����Ϊ���ڵ�������еĺ��ӽڵ㶼�����ķ�Χ��
//����������
//						[1, 6]
//				   /			  \
//			  [1, 3]			   [3, 6]
//			 /      \			 /	      \
//		  [1, 2]   [2, 3]	 [3, 4]	      [4, 6]
//										 /		\
//									  [4, 5]	[5, 6]
//
//�߶η�ΧΪ[1, n]���߶������Ϊlg(n)+1���ռ临�Ӷ�ΪO(n)
//�������߶�����ÿ���ڵ��϶�����һ����־λcover����ǵ�ǰ�ڵ��������䱻Ⱦɫ
//�ڹ����߶���ʱ�ͻ��������нڵ㣬֮��Ĳ��룬ɾ����ͳ�Ʋ���ֻ���û��ȡ��־λcover
//ÿȾɫһ���߶μ����߶����ĸ��ڵ㿪ʼ���������ñ�Ⱦɫ���߶��������߶����еı�־λ
//ͳ�Ƴ���ʱֻ������߶������ۼӱ���ǵĽڵ�����䳤��֮��
//
//���������ˡ��߶���(segment tree)�������ߡ�Microgoogle��

#include "data_structure.h"
#include <iostream>
using namespace std;

//�߶�����ʵ��ʹ�õݹ鼼��
seg_node* segment_tree_build(int beg, int end)
{//����[beg, end]�е����нڵ�
 //���������ɸ��ڵ��beg��mid���߶�����
 //���������ɸ��ڵ��mid��end���߶�����
 //������ͨ�������������һ��
 //�������յõ����߶����ĸ��ڵ�ָ��
	seg_node *root = new seg_node(beg, end);
	if(beg + 1 < end){
		root->s_lc = segment_tree_build(beg, (beg + end) / 2);
		root->s_rc = segment_tree_build((beg + end) / 2, end);
	}
	return(root);
}
void segment_tree_insert(seg_node *root, int beg, int end)
{//����ǰ�ڵ��ѱ�Ⱦɫ�򲻱��ټ������±���
	if(root->s_cover)
		return;

	//ע��������ж�����
	//midָ����ǰ�ڵ��������м��
	int mid = (root->s_beg + root->s_end) / 2;
	if(beg == root->s_beg && end == root->s_end)
		//���������뵱ǰ�ڵ��������ȫ�غϡ�
		root->s_cover = 1;
	else if(end <= mid)
		//����������ҽ��ڵ�ǰ�ڵ���м��mid�����
		//�򽫲���������뵱ǰ�ڵ��������
		segment_tree_insert(root->s_lc, beg, end);
	else if(beg >= mid)
		//�������������ڵ�ǰ�ڵ���м��mid���ұ�
		//�򽫲���������뵱ǰ�ڵ��������
		segment_tree_insert(root->s_rc, beg, end);
	else{
		//���������һ�����ڵ�ǰ�ڵ���м��mid�����
		//��һ�������м��mid���ұ�
		//������������м��mid�ֳ��������֣��ֱ�����ݹ����
		segment_tree_insert(root->s_lc, beg, mid);
		segment_tree_insert(root->s_rc, mid, end);
	}
}
int segment_tree_length(seg_node *root)
{//��ǰ�߶����б����ǵ��߶γ���֮��
	if(root == NULL)
		return(0);
	if(root->s_cover)
		return(root->s_end - root->s_beg);

	int left_length = segment_tree_length(root->s_lc);
	int right_length = segment_tree_length(root->s_rc);
	return(left_length + right_length);
}
void segment_tree_print(seg_node *root)
{//��ӡ�߶�����Ϣ
	if(root == NULL)
		return;
	cout << "node beg:" << root->s_beg << ", end:" << root->s_end << ", cover:" << root->s_cover;
	if(root->s_lc){
		cout << " left child(beg:" << root->s_lc->s_beg << ",end:" << root->s_lc->s_end << ")";
	}
	if(root->s_rc){
		cout << " right child(beg:" << root->s_rc->s_beg << ",end:" << root->s_rc->s_end << ")";
	}
	cout << endl;
	segment_tree_print(root->s_lc);
	segment_tree_print(root->s_rc);
}