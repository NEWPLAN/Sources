
typedef struct student
{
	char num[13];                //ѧ��
	char pro[10];           //רҵ
	char name[10];           //����
	char sex;           //�Ա�
	char chinese,math,english;     //ѧ��
	char tel[12];            //�绰
	char qq[10];            //Q��
	char address[30];      //��ַ
	struct student *next;	 //����ָ��
}STU;

void addstu(STU *stu_first,STU *stu_end,char *gfirst);              //����ѧ����Ϣ
void findstu(STU *stu_first);             //��ѯѧ����Ϣ
void liststu(STU *stu_first);             //ѧ���б�
void modifystu(STU *stu_first, char *gsave);            //�޸�ѧ����Ϣ
void summarystu(STU* stu_first);             //ͳ��ѧ����Ϣ
void delstu(STU *stu_first, char *gsave);                 //ɾ��ѧ��
void resetpwd(char password[]);               //��������
/*modified my newplan*/
void readdata(char* gfirst,STU **stu_first,STU **stu_end);                 //���ļ��л�ȡѧ����Ϣ������������*
void savedata(char* gsave,STU *stu_first);               //�������е����ݱ������ļ���
int modi_grade(int n);          //�޸�������
char *modi_field(char *fieled,char *s ,int n);       //ͨ�������ֶ��޸ĺ���
STU *findname (char *name,STU *stu_first);              //�������������в���
STU *findnum(char *name,STU *stu_first);                  //��ѧ�Ų�ѯ
STU *findtelephone(char *name,STU *stu_first);          //���绰��ѯ
STU *findqq(char *name,STU *stu_first);                     //��Q�Ų�ѯ
void displaystu(STU *stu,char *field,char *name);       //��ʾ����ѧ����Ϣ
void checkfirst(char password[]);                   //�״γ�ʼ��
void bound(char ch,int n);              //��ʾ�ָ���
void login(char password[]);                          //�������
void menu(STU *stu_first,char* gfirst,char* gsave,STU *stu_end,char password[]);                         //��ʾ�˵�
void bound (char x,int n);           //��ʾ�ָ���
