#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000

struct argument{
  int v[SIZE];
  int u[SIZE];

  int start_pos;
  int end_pos;
  int ret;
};

void *compute(void *arg){
  struct argument *a = (struct argument *)arg;
  a->ret = 0;
  for(int i = a->start_pos; i < a->end_pos; i++){
    a->ret += a->u[i] * a->v[i];
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  srand(time(0));
  pthread_t tids[4];

  int v[SIZE];
  int u[SIZE];
  int dotproduct = 0;
  int thread_dotproduct = 0;
  struct argument a[4];
   
  for (int i = 0; i < SIZE; i++) {
    v[i] = rand() % 1000 - 500;
    u[i] = rand() % 1000 - 500;
    for(int j = 0; j < 4; j++){
      a[j].v[i] = v[i];
      a[j].u[i] = u[i];
    }
    dotproduct += u[i] * v[i];
  }

  for(int i = 0; i < 4; i++){
    tids[i] = i;
    a[i].start_pos = (SIZE * i)/4;
    a[i].end_pos = (SIZE * (i + 1))/4;
    pthread_create(&tids[i], NULL, compute, (void *)&a[i]);
  }

  for(int i = 0; i < 4; i++){
    pthread_join(tids[i], NULL);
  }

  for(int i = 0; i < 4; i++){
    thread_dotproduct += a[i].ret;
  }

  printf("Ground truth dot product: %d\n", dotproduct);

  // TODO: Implement your thread solution here
  printf("Test with 4 threads\n");
  printf("Thread dot product: %d\n", thread_dotproduct);

  return 0;
}