//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name: Jac McCarty
// Created Feb. 10, 2022
//
#include <stdio.h>
#include <stdlib.h>

/*
 * returns 1 if inputted 2D matrix is a magic square
 */
int check(int **square, int w, int h){
  int sum = 0;
  if(w != h){
    return -1;
  }
  for(int i = 0; i < w; i++){
    sum += square[0][i];
  }
  
  //check all rows
  for(int i = 0; i < h; i++){
    int newsum = 0;
    for(int j = 0; j < w; j++){
      newsum += square[i][j];
    }
    if(newsum != sum){
      return -1;
    }
  }

  //checks all columns
  for(int j = 0; j < w; j++){
    int newsum = 0;
    for(int i = 0; i < h; i++){
      newsum += square[i][j];
    }
    if(newsum != sum){
      return -1;
    }
  }
  return sum;
}
int main() {
  int width; //width of square
  int height;
  int **square;
  scanf("%d", &width);
  scanf("%d", &height);

  //if not a square, not a magic square
  if(width != height){
    printf("Not a magic square :-(\n");
    exit(1);
  }

  square = malloc(sizeof(int *) * height);
  for(int i = 0; i < width; i++){
    square[i] = malloc(sizeof(int) * width);
  }

  //fill magic square
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      scanf("%d", &square[i][j]);
    }
  }

  //print magic square
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      printf("%-4d ", square[i][j]);
    }
    printf("\n");
  }
  
  //check if magic square
  int magic_constant = check(square, width, height);
  if(magic_constant == -1){
    printf("Not a magic square :-(\n");
  }else{
    printf("M is a magic square (magic constant = %d)\n", magic_constant);
  }
  
  //free all rows
  for(int i = 0; i < height; i++){
    free(square[i]);
    square[i] = NULL;
  }

  //free square
  free(square);
  square = NULL;
}

