//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name: Jac McCarty
// Created: Feb. 12 2022
//
#include <stdio.h>
#include <stdlib.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  struct node *n = malloc(sizeof(struct node));
  n->sym = sym;
  n->linenum = line;
  n->colnum = col;
  n->next = top;
  return n;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
  if(top == NULL){
    return NULL;
  }
  if(top->next == NULL){
    free(top);
    top = NULL;
    return NULL;
  }
  struct node *n = top->next;
  free(top);
  top = NULL;
  return n;
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  if(top == NULL){
    return;
  }
  if(top->next == NULL){
    free(top);
    top = NULL;
    return;
  }
  struct node *loc = top;
  while(loc->next != NULL){
    struct node *temp = loc->next;
    free(loc);
    loc = temp;
  }
  free(loc);
  loc = NULL;
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  if(top == NULL){
    return;
  }
  if(top->next == NULL){
    printf("Unmatched brace on Line %d and Column %d\n", 
        top->linenum, top->colnum);
    return;
  }
  while(top->next != NULL){
    printf("Unmatched brace on Line %d and Column %d\n", 
        top->linenum, top->colnum);
    top = top->next;
  }
  printf("Unmatched brace on Line %d and Column %d\n", 
      top->linenum, top->colnum);
  return;
}

int main(int argc, char* argv[]) {
  FILE *file;
  int isReading = 1;
  int ch;
  int col = 0;
  int line = 1;
  
  struct node *open_top = NULL;

  if(argc <= 1){
    printf("ERROR: must enter file name as command line argument\n");
    //file = fopen("prog1.c", "r");
    exit(1);
  }

  file = fopen(argv[1], "r");

  if(file == NULL){
    printf("\nERROR: unable to open file %s\n", argv[1]);
    //file = fopen("prog1.c", "r");
    exit(1);
  }

  while(isReading == 1){
    ch = getc(file);
    col++;
    if(ch == '{'){
      if(open_top == NULL){
        open_top = malloc(sizeof(struct node));
        open_top->linenum = line;
        open_top->colnum = col;
        open_top->next = NULL;
      }else{
        open_top = push(ch, line, col, open_top);
      }
    }else if(ch == '}'){
      if(open_top == NULL){
        printf("Unmatched brace on Line %d and Column %d\n", line, col);
      }else{
        printf("Found matching braces: (%d, %d) -> (%d, %d)\n", open_top->linenum,
            open_top->colnum, line, col);
        open_top = pop(open_top);
      }
    }else if(ch == '\n'){
      col = 0;
      line++;
    }else if(ch == EOF){
      print(open_top);
      clear(open_top);
      break;
    }
  }
  fclose(file);
  return 0;
}
