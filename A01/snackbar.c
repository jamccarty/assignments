#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack{
  char name[100];
  float cost;
  int quantity;
};

int main(){
  struct snack s[3];
  float money = 0;
  int num = -1;

  strcpy(s[0].name, "Cool Ranch Burrito");
  s[0].cost = 2.30;
  s[0].quantity = 3;

  strcpy(s[1].name, "Textbook -- Edible Edition");
  s[1].cost = 107.99;
  s[1].quantity = 7;  

  strcpy(s[2].name, "Vampire Juice");
  s[2].cost = 6.69;                        
  s[2].quantity = 0;

  printf("\nWelcome to Jac's Snackbar, the only ethical alternative\n");
  printf("to Steven Struct's Snackbar across the hall\n");

  printf("\nHow much money do you have? ");
  scanf("%f", &money);
  printf("\n");

  for(int i = 0; i < 3; i++){
    int len = strlen(s[i].name);
    char tabnum[10];

    if(len > 20){
      strcpy(tabnum, "\t");
    }else{
      strcpy(tabnum, "\t\t");
    }

    printf("%d) %s %s cost: $%.2f \t\t quantity: %d\n", i, s[i].name,
            tabnum, s[i].cost, s[i].quantity);
  }
  printf("\n");

  printf("What snack would you like to buy? [0,1,2] ");
  scanf("%d", &num);
  printf("\n");
  
  if(num < 0 || num > 2){
    printf("Does that look like a 0, 1, or 2 to you?\n");
    return 1;
  }else if(s[num].quantity <= 0){
    printf("Sorry, we're out of %s\n", s[num].name);
  }else if(money < s[num].cost){
    printf("You can't afford it! Sucks to suck lol\n");
    return 1;
  }else{
    printf("You bought %s\n", s[num].name);
    money = money - s[num].cost;
    printf("You have $%.2f left. Spend it wisely\n", money);
  }
  return 1;
}
