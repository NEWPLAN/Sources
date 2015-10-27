#include<iostream>
#include<cstdlib>
#include<vector>
using namespace std;
inline void NEXT(const string&T,vector<int>&next)
{
	//��ģʽ������vector,next(T.size())
	next[0]=-1;
	for(int i=1;i<T.size();i++)
	{
		int j=next[i-1];
		while(T[i]!=T[j+1]&&j>=0)
			j=next[j];//���Ƽ���
	if(T[i]==T[j+1])
		next[i]=j+1;
	else
		next[i]=0;//
	}
}
inline string::size_type COUNT_KMP(const string &S, const string &T)
{
	//����ģʽ��T��next������T������S�еĸ���count��KMP�㷨
	//����T�ǿգ�
	vector<int> next(T.size());
	NEXT(T,next);
	string::size_type index,count=0;
	for(index=0;index<S.size();++index)
	{
	int pos=0;
	string::size_type iter=index;
	while(pos<T.size()&&iter<S.size())
	{
		if(S[iter]==T[pos])
		{
			++iter;
			++pos;
		}
		else
		{
			if(pos==0)
				++iter;
			else
				pos=next[pos-1]+1;
		}
	}//whileend
	if(pos==T.size()&&(iter-index)==T.size())
		++count;
	}//forend
	return count;
}
int main(int argc,char*argv[])
{
	string S="abaabcacabaabcacabaabcacabaabcacabaabcac";
	string T="abc";
	string::size_type count=COUNT_KMP(S,T);
	cout<<count<<endl;
	system("PAUSE");
	return 0;
}