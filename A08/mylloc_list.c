#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int in_use;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }

  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while(next != NULL){
    if(next -> size >= size){
      next->in_use = size;
      if(prev != NULL){
        prev->next = next->next;
      }else{
        flist = next->next;
      }
      return (void*)(next + 1);
    }else{
      prev = next;
      next = next -> next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if(memory == (void *) - 1){
    return NULL;
  }else{
    struct chunk *cnk = (struct chunk*)memory;
    cnk -> size = size;
    cnk -> in_use = size;
    return (void *)(cnk + 1);
  }
}

void free(void *memory) {
  if(memory != NULL){
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory -1);
    cnk -> next = flist;
    flist = cnk;
  }
  return;
}

//Computes:
//  the total number of free chunks allocated
//  the total number of in-use chunks allocated
//  the largest, smallest, and average unused memory across all used chunks
//  the total unused memory across all used chunks
//  the largest, smallest, and average sizes of all free chunks
void fragstats(void* buffers[], int len) {
  struct chunk *freed_loc = flist;

  int in_use_chunks = 0;
  int free_chunks = 0;

  double unused_bytes = 0;
  double total_free_bytes = 0;

  int smallest_unused = 0;
  int smallest_freed = 0;

  int largest_unused = 0;
  int largest_freed = 0;

  double average_unused;
  double average_freed;

  for(int i = 0; i < len; i++){
    if(buffers[i] != NULL){
      struct chunk *ch = (struct chunk*)(buffers[i] - 1);
      int unused = ch->size - ch->in_use;
      unused_bytes += unused;
      if(unused < smallest_unused || smallest_unused == 0){
        smallest_unused = unused;
      }
      if(unused > largest_unused){
        largest_unused = unused;
      }
      in_use_chunks++;
    }
  }

  while(freed_loc != NULL){
    free_chunks++;
    total_free_bytes += freed_loc->size;

    if(freed_loc->size < smallest_freed || smallest_freed == 0){
      smallest_freed = freed_loc->size;
    }
    if(freed_loc->size > largest_freed){
      largest_freed = freed_loc->size;
    }
    freed_loc = freed_loc -> next;
  }

  average_unused = unused_bytes / in_use_chunks;
  average_freed = total_free_bytes / free_chunks;

  printf("Total chunks: %d Free: %d Used: %d\n", in_use_chunks+free_chunks, free_chunks, in_use_chunks);
  printf("Internal unusued: total: %.0f average: %.1f smallest: %d largest: %d\n", unused_bytes, average_unused, smallest_unused, largest_unused);
  printf("External unused: total: %.0f average: %.1f smallest: %d largest: %d\n", total_free_bytes, average_freed, smallest_freed, largest_freed);
}

