#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
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
  int numProcesses = 4;



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
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
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
  }

  int shmid;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size *size, 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel* buffer = shmat(shmid, NULL, 0);
  if (buffer == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 

  struct ppm_pixel** pxs = malloc(sizeof(struct ppm_pixel*) * size);
  for (int i = 0; i < size; i++) {
    pxs[i] = &(buffer[i*size]); 
  }

  int row_start_interval = 0;
  int row_end_interval = size/2;
  int col_start_interval = 0;
  int col_end_interval = size/2;

  int filedes[2], nbytes;

  pid_t pid;
  pipe(filedes);
  pid = fork();

  if(pid == 0){
    row_start_interval = 0;
    row_end_interval = size/2;
    col_start_interval = size/2;
    col_end_interval = size;

    pid = fork();

    if(pid == 0){
      row_start_interval = size/2;
      row_end_interval = size;
      col_start_interval = 0;
      col_end_interval = size/2;

      pid = fork();

      if(pid == 0){
        row_start_interval = size/2;
        row_end_interval = size;
        col_start_interval = size/2;
        col_end_interval = size;
      }
    }
  }

  for(int row = row_start_interval; row < row_end_interval; row++){
    for(int col = col_start_interval; col < col_end_interval; col++){
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

  for (int i = 0; i < 4; i++) {
    free(pallet);
    pallet = NULL;
    if(row_start_interval == size/2 || col_start_interval == size/2){
      exit(0);
    }
    int status;
    int npid = wait(&status);
    printf("Child process complete: %d\n", npid);
  }

  free(pallet);
  pallet = NULL;
  char name[100];
  
  int timestamp = time(0);
  for(int i = 0; i < 100; i++){
    name[i] = '\0';
  }
  sprintf(name, "mmandelbrot-%d-%d.ppm", size, timestamp);
  int len = strlen(name);
  write_ppm(name, pxs, size, size);

  double timeend = time(0);
  end = clock();
  double timetaken = (double)(end-start)/CLOCKS_PER_SEC;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", 
      size, size, timetaken);
  printf("Writing file: %s\n", name);
  // compute image

  free(pxs);
  free(pallet);
  pallet = NULL;
  if (shmdt(buffer) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }   
}