#include<iostream>
using namespace std; 
int main()
{
    bool left=true;
    char c;
    while((c=cin.get())!=EOF){
     /*   
     if(c=='"')  // " �� \" ������ 
      if(left) {cout << "``"; left=false;} //~�� 
      else {cout <<"''"; left=true;} // '�� 
      */
     if(c=='"') {cout<< (left? "``" : "''"); left=!left;}  
       // ?:�������Żᷢ�����ȼ�����������������Ϊ1�����������Ϊ0 
     else cout << c;
    }    
    return 0;
}    
