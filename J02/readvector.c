#include <stdio.h>
#include <stdlib.h>

// readvector opens and reads the given file
// The size of the vector should be stored in the parameter size
// The values should be returned in a flat float array
float* readvector(const char* filename, int *size) {
  FILE *file = fopen(filename,"r");
  char blank;
  fscanf(file, "%d", size);
  float *vals = malloc(sizeof(float) * *size);
  for(int i = 0; i < *size; i++){
    fscanf(file, "%f", &vals[i]); 
    fscanf(file, "%c", &blank);
  } 
  fclose(file);
  file = NULL;
  return vals;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: readvector <filename>\n");
    exit(0);
  }

  int size = 0;
  float* vector = readvector(argv[1], &size);

  for(int i = 0; i < size; i++){
    printf("%f\n", vector[i]);
  }
  free(vector);
  return 0;
}

