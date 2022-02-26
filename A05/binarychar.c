#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char toChar(char* binary){
  int num = 0;
  for(int i = 0; i < 8; i++){
    if(binary[i] == '1'){
      double temp = pow(2.0, (double)7-i);
      num = num + (int)temp;
    }
  }
  return num;
}

int main() {
  char *input = malloc(sizeof(char) * 10);
  char output;

  if(input == NULL){
    printf("Memory error: not enough heap storage\n");
    exit(1);
  }

  printf("Enter a binary number: ");
  scanf("%s", input);
  output = toChar(input);
  printf("\nYour character is: %c\n", output);
  free(input);
  input = NULL;
}
