
typedef struct student
{
	char num[13];                //学号
	char pro[10];           //专业
	char name[10];           //名字
	char sex;           //性别
	char chinese,math,english;     //学科
	char tel[12];            //电话
	char qq[10];            //Q号
	char address[30];      //地址
	struct student *next;	 //链表指针
}STU;

void addstu(STU *stu_first,STU *stu_end,char *gfirst);              //增加学生信息
void findstu(STU *stu_first);             //查询学生信息
void liststu(STU *stu_first);             //学生列表
void modifystu(STU *stu_first, char *gsave);            //修改学生信息
void summarystu(STU* stu_first);             //统计学生信息
void delstu(STU *stu_first, char *gsave);                 //删除学生
void resetpwd(char password[]);               //重设密码
/*modified my newplan*/
void readdata(char* gfirst,STU **stu_first,STU **stu_end);                 //从文件中获取学生信息，创建连链表*
void savedata(char* gsave,STU *stu_first);               //将链表中的数据保存在文件中
int modi_grade(int n);          //修改数字类
char *modi_field(char *fieled,char *s ,int n);       //通过文字字段修改函数
STU *findname (char *name,STU *stu_first);              //按姓名在链表中查找
STU *findnum(char *name,STU *stu_first);                  //按学号查询
STU *findtelephone(char *name,STU *stu_first);          //按电话查询
STU *findqq(char *name,STU *stu_first);                     //按Q号查询
void displaystu(STU *stu,char *field,char *name);       //显示查找学生信息
void checkfirst(char password[]);                   //首次初始化
void bound(char ch,int n);              //显示分割线
void login(char password[]);                          //检查密码
void menu(STU *stu_first,char* gfirst,char* gsave,STU *stu_end,char password[]);                         //显示菜单
void bound (char x,int n);           //显示分隔符
