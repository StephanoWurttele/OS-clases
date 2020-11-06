#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#define N_ITEMS 7

int producer_sleeping = 0;
int consumer_sleeping = 0;


int buffer[N_ITEMS];
int init = 0;
int end = 0;

pthread_cond_t cond_consume;
pthread_cond_t cond_produce;
pthread_mutex_t mutex;

int cont = 0;

void* producer(void *v){
  sleep(2);
  pthread_mutex_lock(&mutex);

  int i;
  for (i=0; i< N_ITEMS * 2; i++){
   if(cont == N_ITEMS){
     producer_sleeping = 1;
     pthread_cond_wait(&cond_produce, &mutex);
     producer_sleeping = 0;
   }
   //while (cont == N_ITEMS); 
   end = (end + 1) % N_ITEMS;
   buffer[end] = i;
   asm("mov cont(%rip), %ebx \n"
       "addl $1, %ebx\n"); // __atomic_fetch_add(....)
   printf("cont is %d", cont);
   if(consumer_sleeping) pthread_cond_signal(&cond_consume);
   printf("producer, item:%d\n",i);
   sleep(random() % 5); // time allow others to exec
   asm("movl %ebx, cont(%rip)\n"); 
  }
  pthread_mutex_unlock(&mutex);
  sleep(2);
return NULL;
}

void* consumer(void *v){
  sleep(2);
  pthread_mutex_lock(&mutex);
  int i;
  int aux;
  for (i=0; i< N_ITEMS * 2; i++){
  // printf("flag\n");
   if(cont == 0) {
     consumer_sleeping = 1;
     pthread_cond_wait(&cond_consume, &mutex);
     consumer_sleeping = 0;
   }
   init = (init + 1) % N_ITEMS;
   aux = buffer[init];
   printf("consumidor, item:%d\n",aux);
   if (aux <i){ // item value consumed before produced
     printf("error: cons bef prod %d\n",aux);}
   else if (aux >i){ // item value produced before space
   	 printf("error: prod bef space %d \n",aux);}
   cont--;
   if(producer_sleeping){
     pthread_cond_signal(&cond_produce);
   }
    pthread_mutex_unlock(&mutex);
   sleep(random() % 5);
  }
return NULL;
}

int	main() {
  pthread_t thread_producer, thread_consumer;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_produce, NULL);
  pthread_cond_init(&cond_consume, NULL);
  
  int i;
  for (i=0; i<N_ITEMS;i++){buffer[i]=-1;}
  
  pthread_create(&thread_producer,NULL, producer, NULL);
  pthread_create(&thread_consumer,NULL, consumer, NULL);
  
  pthread_join(thread_producer,NULL);
  pthread_join(thread_consumer,NULL);
	
  pthread_exit(NULL);
}

/*


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#define N_ITEMS 7

int buffer[N_ITEMS];
int init = 0;
int end = 0;

sem_t position_free;
sem_t position_taken;

void *producer(void *v) {
    for (size_t i = 0; i < N_ITEMS * 2; i++) {
        sem_wait(&position_free);
        end = (end + 1) % N_ITEMS;
        buffer[end] = i;
        printf("producer, item: %d \n", i);
        sem_post(&position_taken);
        sleep(random() % 5);
    }
    return NULL;
}

void *consumer() {
    sleep(2);
    //int aux;
    for (size_t i = 0; i < N_ITEMS * 2; i++) {
        sem_wait(&position_taken);
        init = (init + 1) % N_ITEMS;
        printf("consumer, item: %d \n", aux);
        //if (aux < i)  // item value consumed before produced
          //  printf("error, cons bef prod\n");
        //if (aux > i)  // item value consumed before space
          //  printf("error, prod bef spce\n");
		sem_post(&position_free);
        sleep(random() % 5);
    }
    return NULL;
}


int main() {
  
    sem_init(&position_free,0, N_ITEMS);
    sem_init(&position_taken,0,0);
  
    pthread_t thread_producer, thread_consumer;
    for (size_t i = 0; i < N_ITEMS; i++) buffer[i] = -1;

    pthread_create(&thread_producer, NULL, producer, NULL);
    pthread_create(&thread_consumer, NULL, consumer, NULL);
  
    sem_destroy(&position_free);
    sem_destroy(&position_taken);

    pthread_join(thread_producer, NULL);
    pthread_join(thread_consumer, NULL);

    pthread_exit(NULL);
}
*/
