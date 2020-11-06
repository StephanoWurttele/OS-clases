#include <stdio.h>
#include <pthread.h>

#define N 10


volatile int thread0_executed = 0;
volatile int n_threads_sleeping = 0;

pthread_cond_t cond;
pthread_mutex_t mutex;

void *thread_0(void* value) {
  sleep(2);
  pthread_mutex_lock(&mutex);
  
  int i=0;
  for (i=n_threads_sleeping; i>0;i--){
    pthread_cond_signal(&cond);
    printf("Thread 0 awakes Thread 1\n");
  }
  thread0_executed =1;
  
  pthread_mutex_unlock(&mutex);
	sleep(2); 
  return NULL;
}

void *thread_1(void* value) {
  
  int i=(int) value;
  
  
  
  pthread_mutex_lock(&mutex);
  
  if (!thread0_executed){
    printf("Thread %d go to sleep\n",i);
    n_threads_sleeping++;
    pthread_cond_wait(&cond,&mutex);
    n_threads_sleeping--;
    printf("Thread %d wake up\n",i);
  }
  else
    printf("Thread %d DOES NOT SLEEP\n",i);
  
  sleep(2);
  pthread_mutex_unlock(&mutex);
  return NULL;
}


int main(){

	pthread_t threads[N];
  
	pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
  
  
  pthread_create(&threads[0], NULL, thread_0, NULL);
  
  //for thread creation, 1 to N
  for(int i = 1; i <= N ; i++)
  	pthread_create(&threads[i], NULL, thread_1, (void *)i);
  
  
  //for thread kill, 1 to N
  for(int i = 1;i <= N; i++)
  	pthread_join(threads[i],NULL);
  
  
  pthread_join(threads[0],NULL);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  
  return 0;
}
