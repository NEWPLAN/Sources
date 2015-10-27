/********************************************
 *  教材P53：数制转换算法
 *  对于输入的任意一个非负十进制整数，打印输出等值的base进制数
*********************************************/
typedef int EntryType ;

int  dec2base(int num, int base)
{ 
	EntryType digit;  // 用于接收出栈操作返回的元素值
	StackPtr ps;
	   
	stack_init(ps);    // 构造空栈
	
	// 执行辗转相除操作，将余数入栈
	while ( num){
		stack_push(ps, num%base);
		num = num / base; 
	}
	
	// 按由高位到低位的顺序输出八进制数
	while ( !stack_empty(ps) ){
		stack_pop(ps,  &digit);
		printf(" %d", digit);
	}
	
	stack_destroy(ps);
	return 0;
}