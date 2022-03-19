//Decodes a .ppm binary file to form a message by taking the last
//bit of each pixel color and using them to make up the bits of chars
//
//Created by Jac McCarty, March 4, 2022

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if(argc <= 1){
    printf("Program needs file name as runtime argument\n");
    exit(1);
  }

  char *message; //message to be decoded
  int mes_len; //maximum length of message

  int w = 0; //width of image
  int h = 0; //height of image

  struct ppm_pixel** pxs = read_ppm(argv[1], &w, &h); //build pixel matrix
  mes_len = (w * h * 3)/8;
  message = malloc(sizeof(char) * mes_len + sizeof(char));
  for(int i = 0; i < mes_len; i++){
    message[i] = 0x0;
  }

  int hcount = 0; //primary coordinate in pxs array
  int wcount = 0; //secondary coordinate in pxs array
  int icount = 0; //location in pxs[hcount][wcount] color array
  int charcount = 0; //character location in decoded message
  unsigned char mask = 0x1;
  int pos =  0;
  

  while(hcount != h){
    unsigned char bit;

    if(charcount > mes_len){
      break;
    }

    bit = pxs[hcount][wcount].colors[icount] & mask;
    bit = bit << (7 - pos);
    message[charcount] = message[charcount] | bit;

    icount++;
    pos++;

    if(icount == 3){
      icount = 0;
      wcount++;
    }
    if(wcount == w){
      wcount = 0;
      hcount++;
    }
    if(pos == 8){
      pos = 0;
      charcount++;
    }
  }

  printf("%s\n", message);

  //free all heap space
  for(int i = 0; i < h; i++){
    free(pxs[i]);
    pxs[i] = NULL;
  }
  free(pxs);
  pxs = NULL;
  
  free(message);
  message = NULL;
}

