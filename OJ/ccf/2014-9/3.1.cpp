#include<stdio.h>

#include<ctype.h>

#include<string.h>
#include<stdlib.h>

#define MAXSIZE 10240

#define WORDSIZE 256

int main()

{

    int isCaseSensitive , totalInput , totalLength=0, length;

    int i = 0 ;

    char* line_addr, *lines_ , *line_addr_;

    char lines[MAXSIZE];

    char word[WORDSIZE];

    lines_ = lines;
    scanf("%s\n",word);//����Ҫ���ҵ��ַ���S

    scanf("%d\n",&isCaseSensitive);//�Ƿ��Сд����

    scanf("%d\n",&totalInput);//�������ֵ�����
    for(line_addr=lines; i<totalInput; i++) //�����������
    {
        gets(line_addr);

        length = strlen(line_addr);

        line_addr += length + 1;

        totalLength += length + 1;

    }

//��Сд�����У�ȫ������Сд�Դ�
    if(!isCaseSensitive)
    {

        i = 0;

        while(word[i])
        {
            word[i] = tolower(word[i]);
            i++;
        }
        lines_ = (char*)malloc(totalLength);
        for(i=0 ; i< totalLength; i++)
        {
            lines_[i] = tolower(lines[i]);
        }

    }

    i = 0;

    line_addr = lines ;
    line_addr_ = lines_;
    while(i < totalInput)
    {

        if(strstr(line_addr_,word) != NULL) //�����Ƿ�������ַ���S
        {
            puts(line_addr);
        }

        line_addr += strlen(line_addr) + 1;
        line_addr_ += strlen(line_addr_) + 1;
        i ++ ;

    }
    return 0;
}
