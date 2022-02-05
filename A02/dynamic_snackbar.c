#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

int main() {
  int num;
  struct snack *snackbar;
  printf("Enter a number of snacks: ");
  scanf("%d", &num);
  printf("\n");

  snackbar = malloc(sizeof(struct snack) * num);

  for(int i = 0; i < num; i++){
    printf("Enter a name: ");
    scanf("%s", snackbar[i].name);

    printf("How much does %s cost? ", snackbar[i].name);
    scanf("%f", &snackbar[i].cost);

    printf("How many %ss are you putting in the snackbar? ", 
              snackbar[i].name);
    scanf("%d", &snackbar[i].quantity);
    printf("\n");
  }

  for(int i = 0; i < num; i++){
    printf("\n%-d)\t %-32s cost: $%-10.2f\t quantity: %-d", i, snackbar[i].name,
        snackbar[i].cost, snackbar[i].quantity);
  }
  printf("\n");

  free(snackbar);

}
