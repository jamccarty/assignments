//AUTHOR: Jac McCarty
//CREATED: Feb 15, 2021
//MODIFIED: Feb 18, 2021

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"
 
//Reads an ascii ppm file and returns a 2D array of ppm_pixel's
//param filename - the name of the file to be read
//param w - pointer to the width of ppm, will be assigned value during function
//param h - pointer to the height of ppm, will be assigned value during function
struct ppm_pixel** read_ppm(const char* filename, int *w, int *h) {
  FILE *file = fopen(filename, "r");
  char *whitespace = " \t\f\r\v\n"; //whitespace, separates each char
  char *line; //holds each line of ppm
  char *token; //holds individual parts of line
  int width; //width of ppm (will eventually be stored as w's value)
  int height;//height of ppm (will eventually be stored as h's value)
  struct ppm_pixel **matrix; //2D array to be returned
  unsigned char elems[3] = {0, 0, 0}; //holds red, green, blue from ppm file

  if(file == NULL){
    printf("ERROR: file location %s is invalid", filename);
    return NULL;
  }

  line = malloc(sizeof(char) * 82);

  if(line == NULL){
    printf("ERROR: malloc failed\n");
    exit(1);
  }

  line = fgets(line, 81, file); //ASCII types start with "P3" 
                                //binary ("raw") types start with "P6"
  //if commented lines, skip over
  while(line[0] == '#'){
    line = fgets(line, 81, file);
  }

  //checks to make sure it's in P3 format
  if(line[0] != 'P' && line[1] != '3'){
    printf("ERROR: file %s is not in ASCII format", filename);
    return NULL;
  }

  //getting width and height from ppm
  line = fgets(line, 81, file);

  //if commented lines, still skip over
  while(line[0] == '#'){
    line = fgets(line, 81, file);
  }

  //token for width
  token = strtok(line, whitespace);
  width = atoi(token);

  token = strtok(NULL, whitespace);//gets next token

  //if only one token to be found in line
  if(token == NULL){
    line = fgets(line, 81, file);
    
    //if commented lines, still skip over
    while(line[0] == '#'){
      line = fgets(line, 81, file);
    }

    token = strtok(line, whitespace);
  }

  height = atoi(token);

  //height of matrix
  matrix = malloc(sizeof(struct ppm_pixel*) * height);

  //width of matrix
  for(int i = 0; i < height; i++){
    matrix[i] = malloc(sizeof(struct ppm_pixel) * width);
  }

  //gets next token (this time for max shade of each color)
  token = strtok(NULL, whitespace);
  
  if(token == NULL){
    line = fgets(line, 81, file);
    while(line[0] == '#'){
      line = fgets(line, 81, file);
    }
    token = strtok(line, whitespace);
  }

  int maxval = atoi(token); //just storing; don't really do much with this tbh

  for(int i = 0; i < height; i++){ //for all rows
    for(int j = 0; j < width; j++){ //for each column in each row
      for(int k = 0; k < 3; k++){ //for each char per ascii triplet
        token = strtok(NULL, whitespace);

        //making sure token is valid
        if(token == NULL){
          while(line[0] == '#'){
            line = fgets(line, 81, file);
          }
          line = fgets(line, 81, file);
          token = strtok(line, whitespace);
        }

        //checking for improperly formatted ppm
        if(atoi(token) > maxval){
          printf("ERROR: incorrectly formatted ASCII. Color value cannot");
          printf(" be greater than maximum value %d", maxval);
          exit(1);
        }
        elems[k] = (unsigned char)atoi(token);
      }
      //set values of red, green, blue
      matrix[i][j].red = elems[0];
      matrix[i][j].green = elems[1];
      matrix[i][j].blue = elems[2];
    }
  }
  *w = width;
  *h = height;

  fclose(file);
  free(line);
  line = NULL;

  return matrix;
}

