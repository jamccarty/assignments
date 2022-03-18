//Decodes a .ppm binary file to form a message by taking the last
//bit of each pixel color and using them to make up the bits of chars
//
//Created by Jac McCarty, March 4, 2022

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "read_ppm.h"

//convert 8-bit binary number to char
char convert(unsigned char arr[]){
  char val = 0;
  for(int i = 0; i < 8; i++){
    if(arr[i] == 1){
      val = val + pow(2, 7-i);
    }
  }
  return val;
}

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
  memset(message, '\0', mes_len);

  //holds each character, decoded one bit at a time from the pixel data
  unsigned char ch[9];
  int count = 0; //holds current location within ch array
  int mesloc = 0; //holds current location within message
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      for(int k = 0; k < 3; k++){

        //if reached end of current char, add to message and reset
        if(count == 8){
          message[mesloc] = convert(ch);
          mesloc++;
          count = 0;
        }

        if(pxs[i][j].colors[k] % 2 == 0){
          //if current pixel color is even, add zero to current char
          ch[count] = (unsigned char)0; 
        }else{
          //if odd, add 1 (I don't know why I'm casting instead of
          //just saying '1' or '0' but I'm too afraid to mess with it
          //trust the process
          ch[count] = (unsigned char)1;
        }
        count++; //move to next pixel color, next bit in char
      }
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

