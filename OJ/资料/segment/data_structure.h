//���ݽṹͷ�ļ�
//data_structure.h

//֧�����ݽṹһ�µ��Զ������ݽṹ
#ifndef NULL
#define NULL 0
#endif

struct hash_node{
	//��ϣ��ڵ�
	int h_value;		//�ڵ�ֵ
	hash_node *h_next;	//�ڵ��ڽڵ�ָ��
	hash_node(int cvalue = 0)
		: h_value(cvalue), h_next(NULL){
	}
};
struct seg_node{
	//�߶����ڵ�
	int s_beg;			//�ڵ���߽�
	int s_end;			//�ڵ��ұ߽�
	int s_cover;		//�ڵ��Ƿ񱻸��Ǳ��
	seg_node *s_lc;		//�ڵ����ӽڵ�ָ��
	seg_node *s_rc;		//�ڵ��Һ��ӽڵ�ָ��
	seg_node(int cbeg = 0, int cend = 0)
		: s_beg(cbeg), s_end(cend), s_cover(0), s_lc(NULL), s_rc(NULL){
	}
};
struct dis_node{
	//���鼯�ڵ�
	int d_idx;			//�ڵ��±��
	dis_node *d_fa;		//�ڵ㸸�ڵ�ָ��
	dis_node(int cidx = 0)
		: d_idx(cidx), d_fa(NULL){
	}
};
struct left_node{
	//��ƫ���ڵ�
	int l_idx;			//�ڵ��±��
	int l_dist;			//�ڵ����
	left_node *l_lc;	//�ڵ����ӽڵ�ָ��
	left_node *l_rc;	//�ڵ��Һ��ӽڵ�ָ��
	left_node(int cidx = 0, int cdist = 0)
		: l_idx(cidx), l_dist(cdist), l_lc(NULL), l_rc(NULL){
	}
};