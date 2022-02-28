#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read_ppm.h"

void color_glitch(struct ppm_pixel **pxs, int h, int w){
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      for(int k = 0; k < 3; k++){
        int updown = rand() % 2;
        int bitsh = 1 + (rand() % 2);
        if(updown == 1){
          pxs[i][j].colors[k] = pxs[i][j].colors[k] << bitsh;
        }else{
          pxs[i][j].colors[k] = pxs[i][j].colors[k] << bitsh;
        }
      }
    }
  }
}

int main(int argc, char** argv) {
  srand(time(0));
  if(argc < 2){
    printf("ERROR: program requires file name as runtime argument\n");
    exit(1);
  }

  int w = 0;
  int h = 0;
  int len = strlen(argv[1]);

  struct ppm_pixel** matrix = read_ppm(argv[1], &w, &h);

  char *glitchname = malloc(sizeof(char) * (len + 12));
  glitchname[0] = '\0';
  strcpy(glitchname, argv[1]);

  for(int i = len - 1; i >= len - 4; i--){
    glitchname[i] = '\0';
  }
  strcat(glitchname, "-glitch.ppm");

  color_glitch(matrix, h, w);

  write_ppm(glitchname, matrix, w, h);
  free(glitchname);
  glitchname = NULL;
  for(int i = 0; i < h; i++){
    free(matrix[i]);
    matrix[i] = NULL;
  }
  free(matrix);
  matrix = NULL;
}
