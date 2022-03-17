#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *msg = malloc(sizeof(char) * 51);
  printf("Please enter a phrase: ");
  fgets(msg, 51, stdin);

  int len = strlen(msg);
  char second = 0x02;
  char third = 0x04;

  for(int i = 0; i < len; i++){
    char bit2 = second & msg[i];
    char bit3 = third & msg[i];
    bit2 = bit2 << 1;
    bit3 = bit3 >> 1; 
    
    if(bit2 == 0x0){
      char mask = (-1) * third - 1;
      msg[i] = msg[i] & mask;
    }else{
      msg[i] = msg[i] | bit2;
    }

    if(bit3 == 0x0){
      char mask = (-1) * second - 1;
      msg[i] = msg[i] & mask;
    }else{
      msg[i] = msg[i] | bit3;
    }
  }
  printf("\nscramble: %s\n", msg);
  free(msg);
  msg = NULL;
  return(0);
}
