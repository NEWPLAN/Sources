#include <stdio.h>
#include <string.h>

/*
http://blog.csdn.net/meixr/article/details/6456896#
*/
void GetNextval(char* p, int next[])  ;
int ViolentMatch(char* s, char* p) ;
int KmpSearch(char* s, char* p) ;
void GetNext(char* p,int next[]) ;
int bf(const char *text, const char *find);



int ViolentMatch(char* s, char* p)  
{  
    int sLen = strlen(s);  
    int pLen = strlen(p);  
  
    int i = 0;  
    int j = 0;  
    while (i < sLen && j < pLen)  
    {  
        if (s[i] == p[j])  
        {  
            //�������ǰ�ַ�ƥ��ɹ�����S[i] == P[j]������i++��j++      
            i++;  
            j++;  
        }  
        else  
        {  
            //�����ʧ�䣨��S[i]! = P[j]������i = i - (j - 1)��j = 0      
            i = i - j + 1;  
            j = 0;  
        }  
    }  
    //ƥ��ɹ�������ģʽ��p���ı���s�е�λ�ã����򷵻�-1  
    if (j == pLen)  
        return i - j;  
    else  
        return -1;  
}  


int KmpSearch(char* s, char* p)  
{  
    int i = 0;  
    int j = 0;  
    int sLen = strlen(s);  
    int pLen = strlen(p);  
	int next[20]={0};
	GetNext(p,next);
    while (i < sLen && j < pLen)  
    {  
        //�����j = -1�����ߵ�ǰ�ַ�ƥ��ɹ�����S[i] == P[j]��������i++��j++      
        if (j == -1 || s[i] == p[j])  
        {  
            i++;  
            j++;  
        }  
        else  
        {  
            //�����j != -1���ҵ�ǰ�ַ�ƥ��ʧ�ܣ���S[i] != P[j]�������� i ���䣬j = next[j]      
            //next[j]��Ϊj����Ӧ��nextֵ        
            j = next[j];  
        }  
    }  
    if (j == pLen)  
        return i - j;  
    else  
        return -1;  
}  

void GetNext(char* p,int next[])  
{  
    int pLen = strlen(p);  
    next[0] = -1;  
    int k = -1;  
    int j = 0;  
    while (j < pLen - 1)  
    {  
        //p[k]��ʾǰ׺��p[j]��ʾ��׺  
        if (k == -1 || p[j] == p[k])   
        {  
            ++k;  
            ++j;  
            next[j] = k;  
        }  
        else   
        {  
            k = next[k];  
        }  
    }  
}  

//�Ż������next ������  
void GetNextval(char* p, int next[])  
{  
    int pLen = strlen(p);  
    next[0] = -1;  
    int k = -1;  
    int j = 0;  
    while (j < pLen - 1)  
    {  
        //p[k]��ʾǰ׺��p[j]��ʾ��׺    
        if (k == -1 || p[j] == p[k])  
        {  
            ++j;  
            ++k;  
            //��֮ǰnext�����󷨣��Ķ�������4��  
            if (p[j] != p[k])  
                next[j] = k;   //֮ǰֻ����һ��  
            else  
                //��Ϊ���ܳ���p[j] = p[ next[j ]]�����Ե�����ʱ��Ҫ�����ݹ飬k = next[k] = next[next[k]]  
                next[j] = next[k];  
        }  
        else  
        {  
            k = next[k];  
        }  
    }  
}  

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  bf
 *  Description: brute-force method for string match problem.
 * =====================================================================================
 */
int bf(const char *text, const char *find)
{
    if (text == '/0' || find == '/0')
        return -1;
    int find_len = strlen(find);
    int text_len = strlen(text);
    if (text_len < find_len)
        return -1;
    char *s = text;
    char *p = s;
    char *q = find;
    while (*p != '/0')
    {
        if (*p == *q)
        {
            p++;
            q++;
        }
        else
        {
            s++;
            p = s;
            q = find;
        }
        if (*q == '/0')
        {
            return (p - text) - (q - find);
        }
    }
    return -1;
}

int main(int argc,char** argv)
{
	char * p1="hello world this is newplan @uestc";
	char* p2="newplan";
	int n1=-1,n2=-1;
	n1=ViolentMatch(p1,p2);
	n2=KmpSearch(p1,p2);
	printf("ViolentMatch= %d KmpSearch= %d \n",n1,n2);
	return 0;
}