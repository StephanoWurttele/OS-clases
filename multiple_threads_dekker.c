#include<pthread.h>
#include<stdio.h>

#define N_THREADS 4

volatile int s = 0;
volatile int orden = 3;

void* f_thread(void *v){
  int thread_id = *(int*)v;
  int i;
  for(i=0; i<5; ++i){
    while(orden != thread_id);

    s = thread_id;
    printf("thread %d, s= %d\n", thread_id, s);
    orden = (thread_id + 1) % N_THREADS;
  }
}

int main(){
  pthread_t threads[N_THREADS];
  int id[N_THREADS];

  int i;
  for(i=0; i<N_THREADS;i++){
    id[i]=i;
    pthread_create(&threads[i], NULL, f_thread, &id[i]);
  }
  for(i=0; i<N_THREADS;i++){
    id[i]=i;
    pthread_join(threads[i], NULL);
  }
  return 0;
}
