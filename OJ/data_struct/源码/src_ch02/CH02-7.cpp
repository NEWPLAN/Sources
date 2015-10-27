/*************************************
 *  教材P44：一元多项式的表示和相加 
 *  设指针 pa, pb 分别指向多项式（链表） A,B 中某一结点
 *  将运算结果保存在链表A中
**************************************/

// 定义多项式的项，作为链表的数据元素
typedef struct node { 
	int coef, exp;
	struct node  *next;
}PolyTerm, *ListPtr;

// 多项式加法：pa = pa + pb
void  add_poly(ListPtr pa, ListPtr pb)
{ 
	ListPtr qa, qb;         // qa和qb分别指向pa和pb的当前结点
	ListPtr prev, ptr;     // 临时变量
	int sum;                  // 临时变量，记录当前项的系数和
	
	qa = pa->next;  // qa指向pa头结点后第一个结点
	qb = pb->next;  // qb指向pb头结点后第一个结点
	prev = pa;          // pre指向pa的头结点

	while((qa!=NULL) && (qb!=NULL)) { // qa和qb均非空
		if(qa->exp < qb->exp){
			// qa结点是多项式求和结果中的一项
			// 操作：qa指针后移，保持qb指针不变		
			prev = qa;         // 首先记录qa的当前值		
			qa = qa->next; // qa移动后，prev指向qa的前驱			
		}
		else if( qa->exp == qb->exp){ 
			// qa和qb的指数相同，对其系数求和后进行判断			
			sum = qa->coef + qb->coef;
			if(sum!=0){ 
				qa->coef = sum; // 修改qa结点的系数值
				// 注意：对qa的后移放在if-else语句块之后统一处理
				// 令prev指向当前结点，qa移动后，prev指向qa结点的前驱
				prev = qa;
			}
			else{ // 系数和为零，对qa和qb指向的当前项做“消去”处理
				prev->next = qa->next; // 从链表pa中摘除当前结点
				free(qa);  // 释放qa指向的当前结点的内存空间
			}

			qa = prev->next;  // qa后移
			// 注：无论系数和是否为零，均应释放qb当前结点的空间
			ptr = qb;
			qb = qb->next;     // qb后移		
			free(ptr);     // 释放本轮处理过的qb结点的内存空间
		}
		else{
			// qb结点是多项式求和结果中的一项
			// 操作：将qb结点插入到qa之前，qb指针后移，保持qa指针不变	
			ptr = qb->next;     // 用ptr记录qb指针后移时应指向的位置
			qb->next = qa;      // 将qb结点插入到qa之前
			prev->next = qb;   // 将qb结点插入到qa之前
			prev = qb;              // 令prev指向qa当前结点的前驱（即新插入的qb结点）
			qb = ptr;                // qb指针后移
		}
	}

	if(qb!=NULL){  // pb中元素未处理完，将pb中剩余部分链接到pa之后
		prev->next = qb;  // 注意此时pa=NULL，prev指向表pa的尾结点
	}

	free(pb);  // 表pb中还剩余一个头结点的空间未释放！
}  