#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char* argv[]) {
  clock_t start, end;
  start = clock();
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  struct ppm_pixel *ppx;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

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

  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      float xfrac = (float)col / size;
      float yfrac = (float)row / size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      while(iter < maxIterations && x * x + y * y < 2 * 2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if(iter < maxIterations){
        pxs[row][col].red = pallet[iter].red;
        pxs[row][col].green = pallet[iter].green;
        pxs[row][col].blue = pallet[iter].blue;
       //printf("%d, %d: R: %d G: %d B: %d\n", row, col, (int)pxs[row][col].red, (int)pxs[row][col].green, (int)pxs[row][col].blue);
      }else{
        pxs[row][col].red = 0;
        pxs[row][col].green = 0;
        pxs[row][col].blue = 0;
       //printf("%d, %d: BLACK\n", row, col);
      }
    }
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
}
