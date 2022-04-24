#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "read_ppm.h"

pthread_mutex_t mutex;
pthread_barrier_t bar;
double maxcount;

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
  int **memberships;
  double **counts;
};

void compute_colors(void *arguments){
  struct argument *a = (struct argument *)arguments;
  double gamma = 0.681;
  double factor = 1.0/gamma;

  for(int row = a->row_start; row < a->row_end; row++){
    for(int col = a->col_start; col < a->col_end; col++){
      double value = 0;

      if(a->counts[row][col] > 0){
        value = log(a->counts[row][col]) / log(maxcount);
        value = pow(value, factor);
      }

      a->pxs[row][col].red = (char)(value * 255);
      a->pxs[row][col].green = (char)(value * 255);
      a->pxs[row][col].blue = (char)(value * 255);
    }
  }
}

void step_two(void *arguments){
  struct argument *a = (struct argument *)arguments;
  printf("starting at (%d, %d), ending at (%d, %d)\n", a->col_start, a->row_start, a->col_end, a->row_end);
  for(int row = a->row_start; row < a->row_end; row++){
    for(int col = a->col_start; col < a->col_end; col++){
      if(a->memberships[row][col] == 1){
        continue;
      }
      float xfrac = (float)col / a->size;
      float yfrac = (float)row / a->size;
      float x0 = a->xmin + xfrac * (a->xmax - a->xmin);
      float y0 = a->ymin + yfrac * (a->ymax - a->ymin);

      float x = 0;
      float y = 0;
      while((x * x) + (y * y) < (2 * 2)){
        float xtmp = (x*x) - (y*y) + (x0);
        y = 2*x*y + y0;
        x = xtmp;
        int yrow = round(a->size * (y - a->ymin)/(a->ymax - a->ymin));
        int xcol = round(a->size * (x - a->xmin)/(a->xmax - a->xmin));
        if(yrow < 0 || yrow >= a->size) continue; //out of range
        if(xcol < 0 || xcol >= a->size) continue; //out of range
        
        a->counts[yrow][xcol] += 1;
        pthread_mutex_lock(&mutex);
        if(a->counts[yrow][xcol] > maxcount){
          maxcount = a->counts[yrow][xcol];
        }
        pthread_mutex_unlock(&mutex);
      }
    }
  }
}

void step_one(void *arguments){
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
        a->memberships[row][col] = 0;
      }else{
        a->memberships[row][col] = 1;
      }
    }
  }
}

void *start(void *arguments){
  step_one(arguments);
  step_two(arguments);

  pthread_barrier_wait(&bar);
  compute_colors(arguments);
  return NULL;
}

int main(int argc, char* argv[]) {
  clock_t start_time, end_time;
  start_time = clock();
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
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&bar, NULL, 4);
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

  struct ppm_pixel **pxs = malloc(sizeof(struct ppm_pixel*) * (size));
  int **memberships = malloc(sizeof(int*) * size);
  double **counts = malloc(sizeof(double*) * size);

  for(int i = 0; i < size; i++){
    pxs[i] = malloc(sizeof(struct ppm_pixel) * (size));
    memberships[i] = malloc(sizeof(int) * size);
    counts[i] = malloc(sizeof(double) * size);
    for(int j = 0; j < size; j++){
      memberships[i][j] = 0;
      counts[i][j] = 0;
    }
  }

  maxcount = 0;
  for(int i = 0; i < 4; i++){
    tids[i] = i;
    arguments[i].row_start = 0;
    arguments[i].row_end = size/2;
    arguments[i].col_start = 0;
    arguments[i].col_end = size/2;
    arguments[i].maxIterations = maxIterations;
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
    arguments[i].counts = counts;
    arguments[i].memberships = memberships;
    pthread_create(&tids[i], NULL, start, (void *)&arguments[i]);
  }

  for(int i = 0; i < 4; i++){
    pthread_join(tids[i], NULL);
  }

  pthread_mutex_destroy(&mutex); 
  pthread_barrier_destroy(&bar);
  char name[100];

  int timestamp = time(0);
  for(int i = 0; i < 100; i++){
    name[i] = '\0';
  }
  sprintf(name, "bmandelbrot-%d-%d.ppm", size, timestamp);
  int len = strlen(name);
  write_ppm(name, pxs, size, size);

  for(int i = 0; i < size; i++){
    free(pxs[i]);
    pxs[i] = NULL;
    free(memberships[i]);
    memberships[i] = NULL;
    free(counts[i]);
    counts[i] = NULL;
  }
  free(pxs);
  pxs = NULL;
  free(memberships);
  memberships = NULL;
  free(counts);
  counts = NULL;
  double timeend = time(0);
  end_time = clock();
  double timetaken = (double)(end_time-start_time)/CLOCKS_PER_SEC;
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);
  // todo: your code here
  // compute image
}
