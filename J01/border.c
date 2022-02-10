#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  char word[50];
  char sym;
  int len;

  printf("Please enter a word: ");
  scanf("%s", word);
  printf("\nPlease enter a symbol: ");
  scanf(" %c", &sym);

  len = strlen(word) + 4;
  printf("\n");
  for(int i = 0; i < len; i++){
    printf("%c", sym);
  }

  printf("\n%c %s %c\n", sym, word, sym);

  for(int i = 0; i < len; i++){
    printf("%c", sym);
  }
  printf("\n");
}
