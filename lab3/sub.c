//peterson algorithms
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h> //atomic operation to share between multiple thread 

int data = 0;

atomic_int turn = 0;
atomic_int flag[2] = {0};

void *subchild(void *arg)
{
  for (int i = 0; i < 100000; i++) {
    atomic_store(&flag[0],1); //i want to enter
    atomic_store(&turn,1); // i give chance to orther thread 
    while(atomic_load(&flag[1]) && atomic_load(&turn) == 1); //wait orther to enter and it is their turn 
    data--; // 
    atomic_store(&flag[0],0); // their done 
  }
  pthread_exit(NULL);
}

void *addchild(void *arg)
{
  for (int i = 0; i < 100000; i++) {
    atomic_store(&flag[1],1);
    atomic_store(&turn,0);
    while(atomic_load(&flag[0]) && atomic_load(&turn) == 0);
    data++;
    atomic_store(&flag[1],0);
  }
  pthread_exit(NULL);
}

int main()
{
  pthread_t t1, t2;

  //create 2 thread
  pthread_create(&t1,NULL,addchild,NULL);
  pthread_create(&t2,NULL,subchild,NULL);

  //wait for the thread to finish
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);

  //check the result
  printf("Final result: %d\n",data);

  if (data == 0){
    printf("Success, no race condition\n");
  } else {
    printf("Failed, race condition orcur\n");
  }
  return 0;
}



