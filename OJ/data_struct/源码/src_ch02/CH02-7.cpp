/*************************************
 *  �̲�P44��һԪ����ʽ�ı�ʾ����� 
 *  ��ָ�� pa, pb �ֱ�ָ�����ʽ������ A,B ��ĳһ���
 *  ������������������A��
**************************************/

// �������ʽ�����Ϊ���������Ԫ��
typedef struct node { 
	int coef, exp;
	struct node  *next;
}PolyTerm, *ListPtr;

// ����ʽ�ӷ���pa = pa + pb
void  add_poly(ListPtr pa, ListPtr pb)
{ 
	ListPtr qa, qb;         // qa��qb�ֱ�ָ��pa��pb�ĵ�ǰ���
	ListPtr prev, ptr;     // ��ʱ����
	int sum;                  // ��ʱ��������¼��ǰ���ϵ����
	
	qa = pa->next;  // qaָ��paͷ�����һ�����
	qb = pb->next;  // qbָ��pbͷ�����һ�����
	prev = pa;          // preָ��pa��ͷ���

	while((qa!=NULL) && (qb!=NULL)) { // qa��qb���ǿ�
		if(qa->exp < qb->exp){
			// qa����Ƕ���ʽ��ͽ���е�һ��
			// ������qaָ����ƣ�����qbָ�벻��		
			prev = qa;         // ���ȼ�¼qa�ĵ�ǰֵ		
			qa = qa->next; // qa�ƶ���prevָ��qa��ǰ��			
		}
		else if( qa->exp == qb->exp){ 
			// qa��qb��ָ����ͬ������ϵ����ͺ�����ж�			
			sum = qa->coef + qb->coef;
			if(sum!=0){ 
				qa->coef = sum; // �޸�qa����ϵ��ֵ
				// ע�⣺��qa�ĺ��Ʒ���if-else����֮��ͳһ����
				// ��prevָ��ǰ��㣬qa�ƶ���prevָ��qa����ǰ��
				prev = qa;
			}
			else{ // ϵ����Ϊ�㣬��qa��qbָ��ĵ�ǰ��������ȥ������
				prev->next = qa->next; // ������pa��ժ����ǰ���
				free(qa);  // �ͷ�qaָ��ĵ�ǰ�����ڴ�ռ�
			}

			qa = prev->next;  // qa����
			// ע������ϵ�����Ƿ�Ϊ�㣬��Ӧ�ͷ�qb��ǰ���Ŀռ�
			ptr = qb;
			qb = qb->next;     // qb����		
			free(ptr);     // �ͷű��ִ������qb�����ڴ�ռ�
		}
		else{
			// qb����Ƕ���ʽ��ͽ���е�һ��
			// ��������qb�����뵽qa֮ǰ��qbָ����ƣ�����qaָ�벻��	
			ptr = qb->next;     // ��ptr��¼qbָ�����ʱӦָ���λ��
			qb->next = qa;      // ��qb�����뵽qa֮ǰ
			prev->next = qb;   // ��qb�����뵽qa֮ǰ
			prev = qb;              // ��prevָ��qa��ǰ����ǰ�������²����qb��㣩
			qb = ptr;                // qbָ�����
		}
	}

	if(qb!=NULL){  // pb��Ԫ��δ�����꣬��pb��ʣ�ಿ�����ӵ�pa֮��
		prev->next = qb;  // ע���ʱpa=NULL��prevָ���pa��β���
	}

	free(pb);  // ��pb�л�ʣ��һ��ͷ���Ŀռ�δ�ͷţ�
}  