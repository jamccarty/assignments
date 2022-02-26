#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  printf("Enter 4 characters: ");
  unsigned int num = 0x00000000;
  
  for(int i = 0; i < 4; i++){
    char c;
    unsigned int temp;
    scanf("%c", &c);

    temp = ((unsigned int)c) << (8 * (3-i));
    num = num | temp;

  }

  for(int i = 3; i >= 0; i--){
    unsigned int mask = 0xFF000000;
    mask = mask >> ((3 - i) * 8);
    unsigned int tempch = num | mask;
    char c = (char)(tempch >> ((3 - i) * 8));
    printf("DEBUG: %c = 0x%08x\n", c, num & mask);
  }

  printf("Your number is: %u (0x%08x)\n", num, num);
  return 0;
}
