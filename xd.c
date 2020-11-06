#include<pthread.h>
#include<stdio.h>
#define N_THREADS 4

volatile int s = 0;
volatile int orden = -1;
volatile int interes[N_THREADS]={0,0,0,0};


int exist_thread_interesada(int thread_id) {
	int i;
  for (i = 0;i < N_THREADS;i++) {
    if (i != thread_id && interes[i]) {
      return 1; 
    }
  }
  return 0;  
}

void* f_thread(void *v) {
  int thread_id = *(int*)v;
  
  int i;
  for (i=0; i<10; i++) {
    interes[thread_id]=1;
    while (exist_thread_interesada(thread_id)){
      
      if (orden != thread_id) {
        interes[thread_id] = 0;
        while(orden != -1);
        orden=thread_id;
        
        if (thread_id==3){
          sleep(3);
        }
        if (thread_id==1){
          sleep(1);
        }        
        interes[thread_id] = 1;
      }
    }
    
    s = thread_id;
    printf("thread %d, s= %d\n",thread_id, s);
    orden = -1;
    
    interes[thread_id] = 0;
    
    sleep(1);
  }
	return NULL;
}

int main() {
  pthread_t thread[N_THREADS];
  int id[N_THREADS];
  
  int i;
  for (i = 0; i < N_THREADS; i++) {
    id[i] = i;
    pthread_create(&thread[i],NULL,f_thread, &id[i]);
  }
  for (i = 0;i < N_THREADS; i++) {
    pthread_join(thread[i], NULL);
  }  
  return 0;
}
