#include <stdlib.h>
#include <stdio.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if(argc < 2){
    printf("ERROR: program requires file name as runtime argument");
    exit(1);
  }

  int w = 0;
  int h = 0;
  struct ppm_pixel** matrix = read_ppm(argv[1], &w, &h);

  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      unsigned char r = matrix[i][j].red;
      unsigned char g = matrix[i][j].green;
      unsigned char b = matrix[i][j].blue;

      printf("(%d %d %d) ", r, g, b);
    }
    printf("\n");
  }

  for(int i = 0; i < h; i++){
    free(matrix[i]);
    matrix[i] = NULL;
  }
  free(matrix);
  matrix = NULL;
}

