#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

bool myfunction(int i, int j)
{
	return (i<j);
}

class myclass
{
public :
	bool operator()(int i, int j)
	{
		return (i<j);
	}
}myobject;

void out_function(vector<int>&);

int main(int argc , char** argv)
{
	int myints[]={32,71,12,45,26,80,53,33};
	
	/* 32 71 12 45 26 80 53 33*/
	vector<int> myvector(myints,myints+8);
	out_function(myvector);
	
	/*(12 32 45 71)26 80 53 33*/
	/*using default comparsion (operator<)*/
	sort(myvector.begin(),myvector.begin()+4);
	out_function(myvector);
	
	/* 12 32 45 71(26 33 53 80)*/
	/*using function as comp */
	sort(myvector.begin()+4,myvector.end(),myfunction);
	out_function(myvector);
	
	/*(12 26 32 33 45 53 71 80)*/
	/*using object as comp*/
	sort(myvector.begin(),myvector.end(),myobject);
	out_function(myvector);
	
	return 0;
}

void out_function(vector<int>& myvector)
{
	for(vector<int>::iterator it=myvector.begin();it!=myvector.end(); ++it)
		cout<<' '<<*it;
	cout<<endl;
}
	