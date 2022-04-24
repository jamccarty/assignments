#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"

struct argument{
  int row_start;
  int row_end;
  int col_start;
  int col_end;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int size;
  int maxIterations;
  struct ppm_pixel** pxs;
  struct ppm_pixel* pallet;
};

void *compute(void *arguments){
  struct argument *a = (struct argument *)arguments;
  printf("starting at (%d, %d), ending at (%d, %d)\n", a->col_start, a->row_start, a->col_end, a->row_end);
  for(int row = a->row_start; row < a->row_end; row++){
    for(int col = a->col_start; col < a->col_end; col++){
      float xfrac = (float)col / a->size;
      float yfrac = (float)row / a->size;
      float x0 = a->xmin + xfrac * (a->xmax - a->xmin);
      float y0 = a->ymin + yfrac * (a->ymax - a->ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      while(iter < a->maxIterations && x * x + y * y < 2 * 2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if(iter < a->maxIterations){
        a->pxs[row][col].red = a->pallet[iter].red;
        a->pxs[row][col].green = a->pallet[iter].green;
        a->pxs[row][col].blue = a->pallet[iter].blue;
       //printf("%d, %d: R: %d G: %d B: %d\n", row, col, (int)pxs[row][col].red, (int)pxs[row][col].green, (int)pxs[row][col].blue);
      }else{
        a->pxs[row][col].red = 0;
        a->pxs[row][col].green = 0;
        a->pxs[row][col].blue = 0;
       //printf("%d, %d: BLACK\n", row, col);
      }
    }
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  clock_t start, end;
  start = clock();
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;
  pthread_t tids[4];
  struct argument arguments[4];

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }

  // todo: your work here
  // generate pallet
  srand(time(0));
  char basered = rand() % 255;
  char basegreen = rand() % 255;
  char baseblue = rand() % 255;
  struct ppm_pixel *pallet = malloc(sizeof(struct ppm_pixel) * maxIterations);
  for(int i = 0; i < maxIterations; i++){
    pallet[i].red = basered + (rand() % 100) - 50;
    pallet[i].green = basegreen +(rand() % 100) - 50;
    pallet[i].blue = baseblue + (rand() % 100) - 50;
//    printf("R: %d G: %d B: %d\n", pallet[i].red, pallet[i].green, pallet[i].blue);
  }
  // compute image

  struct ppm_pixel **pxs = malloc(sizeof(struct ppm_pixel*) * (size));
  for(int i = 0; i < size; i++){
    pxs[i] = malloc(sizeof(struct ppm_pixel) * (size));
  }

  for(int i = 0; i < 4; i++){
    tids[i] = i;
    arguments[i].row_start = 0;
    arguments[i].row_end = size/2;
    arguments[i].col_start = 0;
    arguments[i].col_end = size/2;
    arguments[i].maxIterations = maxIterations;
    arguments[i].pallet = pallet;
    arguments[i].size = size;
    arguments[i].xmax = xmax;
    arguments[i].xmin = xmin;
    arguments[i].ymax = ymax;
    arguments[i].ymin = ymin;
    if(i >= 2){
      arguments[i].row_start = size/2;
      arguments[i].row_end = size;
    }

    if(i % 2 == 1){
      arguments[i].col_start = size/2;
      arguments[i].col_end = size;
    }
    arguments[i].pxs = pxs;
    pthread_create(&tids[i], NULL, compute, (void *)&arguments[i]);
  }

  for(int i = 0; i < 4; i++){
    pthread_join(tids[i], NULL);
  }
  free(pallet);
  pallet = NULL;
  char name[100];
  
  int timestamp = time(0);
  for(int i = 0; i < 100; i++){
    name[i] = '\0';
  }
  sprintf(name, "mandelbrot-%d-%d.ppm", size, timestamp);
  int len = strlen(name);
  write_ppm(name, pxs, size, size);

  for(int i = 0; i < size; i++){
    free(pxs[i]);
    pxs[i] = NULL;
  }
  free(pxs);
  pxs = NULL;
  double timeend = time(0);
  end = clock();
  double timetaken = (double)(end-start)/CLOCKS_PER_SEC;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", 
      size, size, timetaken);
  printf("Writing file: %s\n", name);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // generate pallet
  // compute image
}
