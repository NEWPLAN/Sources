/******************************************************************************* 
@All Right Reserved (C), NEWPLAN 
Filepath   : /cDemoOK.cpp
Filename   : cDemoOK.cpp
Version    : ver 1.0
Author     : newplan001@163.com 
Date       : 2015/12/22
Description: test for ICTCLAS LIBS
Notification:apply for Linux 
History    :
1.2015/12/22 14:04 Created by newplan@163.com Version 1.0 
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include  <string>
#include "NLPIR.h"


#define POS_TAGGER_TEST
#ifdef POS_TAGGER_TEST
bool g_bPOSTagged=true;
#else
bool g_bPOSTagged=false;
#endif	
  


void testNLPIR_ParagraphProcess();
void testImportUserDict();

int main()
{
//    testNLPIR_ParagraphProcess();//分词用例	
	testImportUserDict();//导入用户字典
//	getchar();
	return 1;
}


void testNLPIR_ParagraphProcess()
{
	if(!NLPIR_Init(0,1)) //初始化分词组件。
	{
		printf("Init fails\n");  
		return ;
	}
	else
	{
		printf("Init ok\n");
	}

   //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
	NLPIR_SetPOSmap(2);

	char* sSentence=(char*)"随后温总理就离开了舟曲县城，预计温总理今天下午就回到北京。以上就是今天上午的最新动态"; // 需要分词的内容
	unsigned int nPaLen=strlen(sSentence); // 需要分词的长度
	char* sRst=0;   //用户自行分配空间，用于保存结果；
	sRst=(char *)malloc(nPaLen*6); //建议长度为字符串长度的倍。
	int nRstLen=0; //分词结果的长度

	sRst =(char*)NLPIR_ParagraphProcess(sSentence,0);  //字符串处理
	printf("The result is:\n%s\n",sRst);
	//free(sRst);
	//也可以用文件文本导入,调用文件分词接口，将分词结果写入“Test_reslult.txt”文件中
	//NLPIR_FileProcess("Test.txt", "Test_result.txt",CODE_TYPE_GB,1);
	NLPIR_Exit();	//释放资源退出
	return ;  


}




void testImportUserDict()
{
	//初始化分词组件。
	//必须调用此接口后，才能调用其它接口！
	if(!NLPIR_Init(0,1))
	{ 
		printf("NLPIR INIT FAILED!\n");  //初始化失败，退出。
		return ;
	}
    else
    {
        printf("------------Init ok!--------------\n\n");
    }

	char* sSentence=(char*)"随后温总理就离开了舟曲县城，预计温总理今天下午就回到北京。以上就是今天上午的最新动态。";
	int nPaLen=strlen(sSentence);
	int nRstLen=0;
	char* sRst=0;//用户自行分配空间，用于保存结果；


	//未导入用户字典前的处理
	sRst=(char*)NLPIR_ParagraphProcess(sSentence,0); 
	printf("--------添加用户字典前分词结果--------\n%s\n\n",sRst);
	//free(sRst);

	printf("---------------添加词典---------------\n");
	const char* pszDictBuffer="userdict.txt"; //字符串形式导入用户词典
	int nLen=strlen(pszDictBuffer);
	unsigned int nItems=NLPIR_ImportUserDict(pszDictBuffer);
	//也可以将用户词汇写入文件，词语之间用换行符隔开
	//unsigned int nItems=NLPIR_ImportUserDictFile("userdict.txt",CODE_TYPE_UNKNOWN);
	printf("\n添加了%d 个用户词\n\n",nItems); //文件形式导入用户字典

	char* sRst1=0;
	sRst1=(char *)malloc(nPaLen*6);
	sRst1=(char*)NLPIR_ParagraphProcess(sSentence,0);//导入用户字典后分词处理
	printf("---------添加用户字典后分词结果-------\n%s\n",sRst1);

	NLPIR_SaveTheUsrDic(); //保存添加的用户词汇,下次使用依然有效
	//释放资源退出
	NLPIR_Exit();
	return ;
}

