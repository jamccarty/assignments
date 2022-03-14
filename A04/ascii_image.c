#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  struct ppm_pixel **matrix;
  int w;
  int h;
  int scale;
  
  //different color grades assigned to printed ppm 
  //depending on ascii color values
  char color_grades [10] = {'@', '#', '%', '*', 'o', ';', ':', ',', '.',' '};

  //if not argument inputted as runtime argument
  if(argc <= 1){
    printf("ERROR: program requires file name as runtime argument");
    exit(1);
  }

  matrix = read_ppm(argv[1], &w, &h); //initialize matrix

  //print associated color_grades of each ppm_pixel in matrix
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      unsigned int r = matrix[i][j].red;
      unsigned int g = matrix[i][j].green;
      unsigned int b = matrix[i][j].blue;

      unsigned int intensity = (r + g + b)/3; //finding intensity of each pixel

      //placing intensity on color_grade scale
      scale = (int)((intensity) - 1) / 25;

      //if scale is too big (like if 251 <= scale <= 255)
      if(scale >= 9){
        scale = 9;
      }
      
      printf("%c", color_grades[scale]);
    }

    printf("\n"); //printing para break to separate each row
  }
  for(int i = 0; i < h; i++){
    free(matrix[i]);
    matrix[i] = NULL;
  }
  free(matrix);
  matrix = NULL;
  return 0;
}

