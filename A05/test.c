#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
  int x = 55;
  char xstr[10];
  itoa(x, xstr, 10);
  int xlen = strlen(xstr);
  printf("%d", xlen);
}
