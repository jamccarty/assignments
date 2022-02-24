#include <stdio.h>
#include <string.h>

struct cake {
  float cost;
  char flavor[16];
  struct cake* next;
};

struct cake makeCake(const char* flavor, float cost) {
  struct cake newCake;
  strncpy(newCake.flavor, flavor, 16);
  newCake.cost = cost;
  newCake.next = NULL;
  return newCake;
}

// todo: implement cheapestCake
void cheapestCake(struct cake* head){
  struct cake *cheapest = head;
  struct cake *loc = head;
  if(head == NULL){
    printf("linked list is empty");
    return;
  }
  while(loc != NULL){
    printf("cake: %s ($%.2f)\n", loc->flavor, loc->cost);
    if(loc->cost < cheapest->cost){
      cheapest = loc;
    }
    loc = loc->next;
  }
  printf("The cheapest cake is %s\n", cheapest->flavor);
}

int main() {
  struct cake cake1 = makeCake("red velvet", 2.00);
  struct cake cake2 = makeCake("chocolate", 1.75);
  struct cake cake3 = makeCake("mocha", 3.50);
  // todo: define 4th cake

  cake1.next = &cake2;
  cake2.next = &cake3;

  struct cake cake4 = makeCake("vanilla", 17.00);
  cake3.next = &cake4;
  // draw stack and heap here

  cheapestCake(&cake1);
}
