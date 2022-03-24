#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  char *msg = malloc(sizeof(char) * 1024);
  printf("Please enter a phrase: ");
  fgets(msg, 1023, stdin);
  msg[strcspn(msg, "\n")] = 0;
  int len = strlen(msg);
  char second = 0x02;
  char third = 0x04;
  
  for(int i = 0; i < len; i++){
    if(((msg[i] & second) != second) ^ ((msg[i] & third) != third)){
      msg[i] = (msg[i] ^ second) ^ third;
    }
  }
  printf("scramble: %s\n", msg);
  free(msg);
  msg = NULL;
  return(0);
}
