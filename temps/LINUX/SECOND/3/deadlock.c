#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

#define NUM 		5
#define THINKING 	0
#define EATING 		1
#define LEFT 		(i-1+NUM)%NUM
#define RIGHT 		(i+1)%NUM
pthread_mutex_t 	mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutexattr_t attr;

int state[NUM];



void* philosopher(void *pid);
void think(int i);
void eat(int i);
void get_chopstick(int i);
void free_chopstick(int i);
void test(int i);




int main(void)
{
	int ncount=0;
	int child[5]={1,2,3,4,5};
	pthread_t pid[5];
	for(ncount=0;ncount<NUM;ncount++)
	{
		pthread_create(&pid[ncount],NULL,philosopher,(void*)(&child[ncount]));
	}
	for(ncount=0;ncount<5;ncount++)
	{
		pthread_join(pid[ncount],NULL);
	}
	return 0;
}





void* philosopher(void *pid)
{
	while(1)
	{
		//printf("thread %d !\n", (int)(*(int*)pid));
		//sleep( (int)(*(int*)pid));
		think((int)(*(int*)pid));
		get_chopstick((int)(*(int*)pid));
		eat((int)(*(int*)pid));
		free_chopstick((int)(*(int*)pid));
	}
	return NULL;
}


void think(int i)
{
	if(state[i]==THINKING)
	printf("philosopher %d is thinking\n",i);
	sleep(i);
	return ;
}

void eat(int i)
{	
	if(state[i]==EATING)
	{
		printf("philosopher %d is eating\n",i);
	}
	sleep(i);
	return ;
}

void get_chopstick(int i)
{
	pthread_mutex_lock(&mutex);
	test(i);
	pthread_mutex_unlock(&mutex);
	return ;
}
void free_chopstick(int i)
{
	state[i]=THINKING;
	return ;
}

void test(int i)
{
	if(state[LEFT]!=EATING && state[RIGHT]!=EATING)
	{
		state[i]=EATING;
	}
	return ;
}

