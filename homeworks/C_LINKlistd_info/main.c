#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "stu.h"

int main(void)
{

char password[9];              //��½����
STU *stu_first=NULL, *stu_end=NULL;      //������β��ָ��*
char gsave,gfirst;

//stu_first=stu_end=NULL;
gsave=gfirst=0;

checkfirst(password);
login(password);
/*modified my newplan*/
readdata(&gfirst,&stu_first,&stu_end);
menu(stu_first,&gfirst,&gsave,stu_end, password);
return 0;
}



