#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *word;
  int num;

  word = malloc(sizeof(char) * 32);

  printf("Enter a word: ");
  scanf("%s", word);
  printf("\nEnter a count: ");
  scanf("%d", &num);
  printf("\n");

  for(int i = 0; i < num; i++){
    printf("%s", word);
  }
  free(word);
}
