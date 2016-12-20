/********************************************
 *  �̲�P53������ת���㷨
 *  �������������һ���Ǹ�ʮ������������ӡ�����ֵ��base������
*********************************************/
typedef int EntryType ;

int  dec2base(int num, int base)
{ 
	EntryType digit;  // ���ڽ��ճ�ջ�������ص�Ԫ��ֵ
	StackPtr ps;
	   
	stack_init(ps);    // �����ջ
	
	// ִ��շת�����������������ջ
	while ( num){
		stack_push(ps, num%base);
		num = num / base; 
	}
	
	// ���ɸ�λ����λ��˳������˽�����
	while ( !stack_empty(ps) ){
		stack_pop(ps,  &digit);
		printf(" %d", digit);
	}
	
	stack_destroy(ps);
	return 0;
}