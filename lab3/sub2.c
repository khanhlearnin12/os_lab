//semaphore
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sched.h> //help cpu to schedule their work 
typedef struct global_data
{
	int data;
	int count;
	atomic_flag m;  
} global_data;
global_data gdata;

void semwait();
void semsignal();
void *subchild(void *arg);
void *addchild(void *arg);

int main()
{
  pthread_t t1, t2;
  
  //create the thread
  pthread_create(&t1,NULL,addchild,NULL);
  pthread_create(&t1,NULL,subchild,NULL);
  
  //join thread 
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  
  //print data 
  printf("Final Data Value: %d\n",gdata.data);

  return 0;
}

void semwait()
{
	while (atomic_flag_test_and_set(&gdata.m))
	{
    sched_yield(); //pause breiffly to save CPU
                   //this schdule help cpu to process if return 0 is success if -1 is not 
  }
}

void semsignal()
{
  atomic_flag_clear(&gdata.m);
}

void *subchild(void *arg){
  for (int i = 0; i < 10000; i++) { 
      semwait();//lock
      gdata.data--; //critical section
      semsignal();//unclock 
  }
  return NULL;
}

void *addchild(void *arg){
  for (int i = 0 ; i < 10000;i++) { 
    semwait(); //lock   
    gdata.data++; //critical section 
    semsignal();//unlock
  }
  return NULL;
}

