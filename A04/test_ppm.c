#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>

int main() {
  struct ppm_pixel **matrix;
  int *w = malloc(sizeof(int)); //width of matrix
  int *h = malloc(sizeof(int)); //height of matrix

  matrix = read_ppm("feep-ascii.ppm", w, h); //initialize matrix

  //print matrix
  for(int i = 0; i < *h; i++){
    for(int j = 0; j < *w; j++){
      printf("%-3d %-3d %-3d\t", matrix[i][j].red, matrix[i][j].green, 
              matrix[i][j].blue);
    }
    printf("\n");
  }
  
  //free matrix
  for(int i = 0; i < *h; i++){
    free(matrix[i]);
    matrix[i] = NULL;
  }
  free(matrix);
  matrix = NULL;

  free(h);
  h = NULL;
  free(w);
  w = NULL;
  return 1;
}

