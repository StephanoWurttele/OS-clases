#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Compile: gcc thread-dekker.c -o thread-dek.out -lpthread */

volatile int s = 0;
volatile int order = 0;
volatile int interest[2] = {0, 0};

void* fthread0(void* v) {
int i;
  for (i = 0; i < 4; ++i) {
    interest[0] = 1; // Indica interes
    sleep(2);
    while (interest[1]) { // Mientras el otro thread ejecuta
      if (order != 0) {
        interest[0] = 0;
        while (order != 0); // Verifica orden
        interest[0] = 1;
      }
    }
    s = 0;
    printf("Thread 0 -> s:%d\n", s);
    order = 1;
    interest[0] = 0;
  }
}

void* fthread1(void* v) {
  int i;
  for (i = 0; i < 4; ++i) {
    interest[1] = 1; // Indica interes
    sleep(2);
    while (interest[0]) { // Mientras el otro thread ejecuta
      if (order != 1) {
        interest[1] = 0;
        while (order != 1); // Verifica orden
        interest[1] = 1;
      }
    }
    s = 1;
    printf("Thread 1 -> s:%d\n", s);
    order = 0;
    interest[1] = 0;
    sleep(1);
  }
}

int main() {

  pthread_t thread0, thread1;
  pthread_create(&thread0, NULL, fthread0, NULL);
  pthread_create(&thread1, NULL, fthread1, NULL);
  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);

  return 0;
} 
