#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

volatile int s = 0;
volatile int order = 0;
volatile int interest[2] = {0, 0};
void* fthread_0(void *v){
  int i;
  for(i=0;i<4;++i){
    interest[0] = 1;
    sleep(2);
    while(interest[1]){
      interest[0] = 0;
      while(order != 0);
      interest[0] = 1;
    }
    s=0;
    order = 1;
    interest[0] = 0;
    printf("Thread0, s:%d\n",s);

  }
  return NULL;
}

void* fthread_1(void *v){
  int j;
  for(j=0;j<4;++j){
    interest[1] = 1;
    sleep(2);
    while(interest[0]){
      interest[1] = 0;
      while(order != 1);
      interest[1] = 1;
    }
    s=1;
    printf("Thread1, s:%d\n",s);
    order = 0;
    interest[1] = 0;
    sleep(1);
  }
  return NULL;
}

int main(){
  pthread_t thread_0, thread_1;
  pthread_create(&thread_0, NULL, fthread_0, NULL);
  pthread_create(&thread_1, NULL, fthread_1, NULL);

  pthread_join(thread_0, NULL);
  pthread_join(thread_1, NULL);
 
  return 0;
}
