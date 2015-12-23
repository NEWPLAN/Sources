#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "ICTCLAS50.h"
int main()
{

	//初始化分词组件。
	//必须调用此接口后，才能调用其它接口！
	if(!ICTCLAS_Init())
	{//初始化失败，退出。
		printf("ICTCLAS INIT FAILED!\n");
		system("pause");
		return 0;
	}
	//char* sSentence="随后温总理就离开了舟曲县城，预计温总理今天下午就回到北京。以上就是今天上午的最新动态。";
	char sSentence[100]={0};
	FILE* pp=fopen("111.txt","rb");
	fread(sSentence,1,100,pp);
	//char* sSentence="连接至 G-BOOK 中心后１０号高度控制管卡夹门控灯开关机油泵进油管";
		//char* sSentence="连接至 G-BOOK 中心后１０号高度控制管卡夹门控灯开关机油泵进油管";
	int nPaLen=100;
	//int nPaLen=strlen(sSentence);
	int nRstLen=0;
	fclose(pp);
		/*
	char* sRst=0;//用户自行分配空间，用于保存结果；
	sRst=(char *)malloc(nPaLen*6);//建议长度为字符串长度的6倍。
	//nRstLen=ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst,CODE_TYPE_UNKNOWN,true); //未导入用户字典前的处理
	//printf("Before Adding User-defined lexicon, the result is:\n%s\n",sRst);
	//free(sRst);
   //"1989年春夏之交 n##政治风波年 a##ABC防护训练 n##test ss";
	//char * pUserWords="门控灯开关@@MT;１０号高度控制管卡夹@@EPC;门控灯开关@@MT;机油泵进油管@@MT;";
	*/
	char * pUserWords="灯开@@sss";
	int nLen=strlen(pUserWords);
	unsigned int nItems=0;
	nItems=ICTCLAS_ImportUserDict(pUserWords,nLen,CODE_TYPE_UNKNOWN);//导入用户字典
	printf("%d user-defined lexical entries added!\n",nItems);
	char* sRst1=0;
	sRst1=(char *)malloc(nPaLen*6);
	nRstLen=ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst1,CODE_TYPE_UNKNOWN,true); //未导入用户字典前的处理
	printf("After Adding User-defined lexicon, the result is:\n%s\n",sRst1);
	free(sRst1);
	//释放资源退出
	ICTCLAS_Exit();
	return 0;
}
