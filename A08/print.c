#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init = sbrk(0);
  printf("The initial top of the heap was 0x%p\n", init);
  
  //printf("The initial top of the heap was 0x%p\n", init);
  void *current = sbrk(0);
  
  printf("The current top of the heap is 0x%p\n", current);

  printf("Increased by %ld (0x%ld) bytes\n", current-init, current-init);
}
