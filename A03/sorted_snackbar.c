//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name: Jac McCarty
// Created 2/10/2022
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};

// Insert a new node to a list (implemented as a linked list). 
// The new node should store the given properties
// Param head: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list
struct snack* insert_sorted(struct snack* head, 
      const char* name, int quantity, float cost) {

  int len = strlen(name);
  char formatted_name[len+1];
  memset(formatted_name, '\0', sizeof(formatted_name));

  if(len > 0){
    formatted_name[0] = toupper(name[0]);
  }
  for(int i = 1; i < len; i++){
    formatted_name[i] = tolower(name[i]);
  }
  
  struct snack *s = malloc(sizeof(struct snack));
  if(s == NULL){
    printf("ERROR: out of space!\n");
    exit(1);
  }

  //copy all snack variables to s
  strcpy(s->name, formatted_name);
  s->quantity = quantity;
  s->cost = cost;

  //if linked list is empty, make s new head
  if(head == NULL){
    s->next = NULL;
    return s;
  }

  //if linked list has only one item in it, set
  //s as either next part or as new head
  if(head->next == NULL){
    int cmp = strcmp(s->name, head->name);
    if(cmp >= 0){
      head->next = s;
      s->next = NULL;
      return head;
    }else{
      s->next = head;
      return s;
    }
  }

  if(strcmp(s->name, head->name) < 0){
    s->next = head;
    return s;
  }

  //places s in appropriate alphabetical spot
  struct snack* prev = head;
  while(prev->next != NULL){
    //if s->name > prev->next->name
    if(strcmp(s->name, prev->next->name) >= 0){
      prev = prev->next;
    }else{
      struct snack* temp = prev->next;
      prev->next = s;
      s->next = temp;
      break;
    }
  }
  //if reaches end of linked list, then places s at end
  if(prev->next == NULL && strcmp(s->name, prev->name) > 0){
    prev->next = s;
    s->next = NULL;
  }
  return head;
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param head: the first node in the list (NULL if empty)
void clear(struct snack* head) {
  if(head == NULL){
    return;
  }

  //if only one item in list, frees it
  if(head->next == NULL){
    free(head);
    head = NULL;
    return;
  }

  //frees everything in linked list
  struct snack *loc = head;
  while(loc->next != NULL){
    struct snack *temp = loc->next;
    free(loc);
    loc = temp;
  }

  free(loc);
  loc = NULL;
}

//prints everything in linked list in appropriate format
//param snacks: the first snack in the linked list (the head)
void print(struct snack *snacks){
  int count = 0;
  while(snacks != NULL){
      printf("\n%d) %-20s\t cost: $%-.2f \tquantity: %-d", count,
        snacks->name, snacks->cost, snacks->quantity);
      count++;
      snacks = snacks->next;
  }
  printf("\n");
}
      
    

int main() {
  struct snack *head = NULL;
  int num = 0;
  printf("\nPlease enter the number of snacks you would like to add: ");
  scanf("%d", &num);

  if(num > 0){
    char name[32];
    float cost;
    int quantity;
    printf("Please enter the name of your snack: ");
    scanf("%s", name);
    printf("Please enter the cost of your snack: ");
    scanf("%f", &cost);
    printf("Please enter the quantity of snacks you are adding: ");
    scanf("%d", &quantity);
    head = insert_sorted(NULL, name, quantity, cost);
  }

  for(int i = 1; i < num; i++){
    char name[32];
    float cost;
    int quantity;
    printf("\nPlease enter the name of your snack: ");
    scanf("%s", name);
    printf("Please enter the cost of your snack: ");
    scanf("%f", &cost);
    printf("Please enter the quantity of snacks you are adding: ");
    scanf("%d", &quantity);
    printf("\n");

    head = insert_sorted(head, name, quantity, cost);
  }
  print(head);
  clear(head);
  return 0;
}

