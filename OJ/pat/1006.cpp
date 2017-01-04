#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>

using namespace std;
typedef struct TIME
{
	int hh,mm,ss;
}Time;
typedef struct STAFF
{
	string name;
	Time come_time,exit_time;
	int in_time,out_time;
	void standard(void)
	{
		in_time=come_time.hh*60*60+come_time.mm*60+come_time.ss;
		out_time=exit_time.hh*60*60+exit_time.mm*60+exit_time.ss;
	}
	
}staff;
void inits(staff * p)
{
	p->come_time.hh=23;
	p->come_time.mm=59;
	p->come_time.ss=59;
	p->exit_time.hh=0;
	p->exit_time.mm=0;
	p->exit_time.ss=0;
	p->standard();
}

int main()
{
	int n;
	staff first_person,last_person,temp_person;
	inits(&first_person);
	inits(&last_person);
	cin>>n;
	while(n--)
	{
		cin>>temp_person.name;
		scanf("%d:%d:%d",&(temp_person.come_time.hh),&(temp_person.come_time.mm),&(temp_person.come_time.ss));
		scanf("%d:%d:%d",&(temp_person.exit_time.hh),&(temp_person.exit_time.mm),&(temp_person.exit_time.ss));
		temp_person.standard();
		if(first_person.in_time>temp_person.in_time)
			first_person=temp_person;
		if(last_person.out_time<temp_person.out_time)
			last_person=temp_person;
	}
	cout<<first_person.name<<" "<<last_person.name;
	return 0;
}