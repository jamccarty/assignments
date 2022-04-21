#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000

struct arg{
  int u[SIZE];
  int v[SIZE];
  int start;
  int end;
  int *dotproduct;
};

void* compute(void* args){
  struct arg a = *((struct arg *)args);
  for (int i = a.start; i < a.end; i++) {
    *a.dotproduct += a.u[i] * a.v[i];
    if(i % (SIZE/4) == 0){
      printf("%d\n", *a.dotproduct);
    }
  }
  return (void *)a.dotproduct;
}

int main(int argc, char *argv[]) {
  srand(time(0));
  pthread_t threads[4];
  int ids[4];

  int v[SIZE];
  int u[SIZE];
  int dotproduct;
  dotproduct = 0;
  int products[4];
  struct arg a[4];
  for(int i = 0; i < 4; i++){
    a[i].start = i * SIZE/4;
    a[i].end = a[i].start + SIZE/4;
    a[i].dotproduct = &products[i];
  }

   
  for (int i = 0; i < SIZE; i++) {
    v[i] = rand() % 1000 - 500;
    u[i] = rand() % 1000 - 500;
    for(int j = 0; j < 4; j++){
      a[j].v[i] = v[i];
      a[j].u[i] = u[i];
    }
    dotproduct += u[i] * v[i];
  }
  printf("Ground truth dot product: %d\n", dotproduct);

  for(int i = 0; i < 4; i++){
    pthread_create(&threads[i], NULL, compute, &a);
  }

  for(int i = 0; i < 4; i++){
    pthread_join(threads[i], (void **)&products);
  }

  for(int i = 0; i < 4; i++){
    dotproduct += products[i];
    printf("%d\n", products[i]);
  }

  // TODO: Implement your thread solution here
  printf("Test with 4 threads\n");
  printf("With 4 threads: dotproduct = %d\n", dotproduct);

  return 0;
}
