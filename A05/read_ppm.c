#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  FILE *file = fopen(filename, "rb");
  char *whitespace = " \t\f\r\v\n";
  char *line;
  char *token;
  int width;
  int height;
  struct ppm_pixel **matrix;

  if(file == NULL){
    printf("ERROR: file location %s is invalid", filename);
    exit(1);
  }

  line = malloc(sizeof(char) * 82);

  if(line == NULL){
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  line = fgets(line, 81, file);
  while(line[0] == '#'){
    line = fgets(line, 81, file);
  }

  if(line[0] != 'P' && line[1] != '6'){
    printf("ERROR: file %s is not raw\n", filename);
    exit(1);
  }
  line = fgets(line, 81, file);

  while(line[0] == '#'){
    line = fgets(line, 81, file);
  }

  token = strtok(line, whitespace);
  width = atoi(token);

  token = strtok(NULL, whitespace);

  if(token == NULL){
    line = fgets(line, 81, file);
    while(line[0] == '#'){
      line = fgets(line, 81, file);
    }
    token = strtok(line, whitespace);
  }
  height = atoi(token);
  
  matrix = malloc(sizeof(struct ppm_pixel*) * height);

  for(int i = 0; i < height; i++){
    matrix[i] = malloc(sizeof(struct ppm_pixel) * width);
  }

  token = strtok(NULL, whitespace);

  while(token == NULL){
    line = fgets(line, 81, file);
    token = strtok(line, whitespace);
  }

  int maxval = atoi(token);

  if(maxval != 255){
    printf("ERROR: maximum color value must be 255\n");
    exit(1);
  }

  unsigned char *elems = malloc(sizeof(char) * (width * height * 3 + 1));
  for(int i = 0; i < (width * height * 3) + 1; i++){
    elems[i] = '\0';
  }
  //memset(elems, '\0', width * height + 1);
  fread(elems, sizeof(unsigned char), width * height * 3, file);

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      for(int k = 0; k < 3; k++){
        matrix[i][j].colors[k] = elems[(i * width * 3) + (j * 3) + k];
      }
    }
  }
  *w = width;
  *h = height;
  free(elems);
  free(line);
  fclose(file);
  return matrix;
}

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
extern void write_ppm(const char* filename, struct ppm_pixel** pxs, int w, int h) {
  FILE *file = fopen(filename, "wb");

  if(file == NULL){
    printf("ERROR: could not find file %s\n", filename);
    exit(1);
  }

  int wlen = 0;
  int hlen = 0;
  int width = w;
  int height = h;
  
  while(width != 0){
    wlen++;
    width = width / 10;
  }

  while(height != 0){
    hlen++;
    height = height / 10;
  }

  char *info = malloc(sizeof(char) * (55 + wlen + hlen + 1));
  info = memset(info, '\0', 55+wlen+hlen);
  sprintf(info, "P6\n# Created by GIMP version 2.10.24 PNM plug-in\n%d %d\n255\n", w, h);
  fwrite(info, sizeof(char), 55 + wlen + hlen, file);

  unsigned char *elems = malloc(sizeof(char) * (w * h * 3) + sizeof(char));

  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      for(int k = 0; k < 3; k++){
        elems[(i * w * 3) + (j * 3) + k] = (unsigned int)pxs[i][j].colors[k];
      }
    }
  }
  elems[(h * w * 3)] = '\n';
  fwrite(elems, sizeof(char), (w * h * 3) + 1, file);

  fclose(file);
  free(elems);
  free(info);
}
