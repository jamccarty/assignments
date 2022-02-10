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
  int len = strlen(word);

  char *repeated;
  repeated = malloc(sizeof(char) * 32 * num);
  strcpy(repeated, word);

  for(int i = 0; i < num; i++){
    strncat(repeated, word, len);
  }
  strcat(repeated, "\0");
  printf("%s", repeated);
  free(word);
  word = NULL;
  free(repeated);
  repeated = NULL;
}
