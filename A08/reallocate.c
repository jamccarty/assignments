#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init = sbrk(0);
  //printf("Initial Heap: %p\n", init);
  void *current;
  for(int i = 0; i < 10; i++){
    char *x = malloc(sizeof(char)*100);
    
    for(int j = 0; j < 99; j++){
      x[j] = (j % 10);
    }
    x[99] = '\0';

    free(x);
    x = NULL;
  }
  current = sbrk(0);
  //printf("Current Heap: %p\n", current);
  printf("Heap grew 0x%ld\n", current-init);
}
