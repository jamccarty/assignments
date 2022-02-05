/*
  Generates a random word and prompts user to guess each letter

  Author: Jac McCarty
  Created: Feb. 2, 2022
  Last Modified: Feb. 4, 2022
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
  returns number of times char has been guessed (CORRECTLY)

  param char *correct_chars: string of correct chars guessed by user
                             only checks to see if ch is there, so other chars
                             outside of guessed okay too (like '_' and ' ')
  param int num_chars_guessed: number of characters correctly guessed by user
                               should be the size of the correct_chars array
  param char ch: the char searching correct_chars for
*/
int timesGuessed(char *correct_chars, int num_chars_guessed, char ch){
  int count = 0;
  for(int i = 0; i < num_chars_guessed; i++){
    if(ch == correct_chars[i]){
      count++;
    }
  }
  return count;
}

/*

  searches word for char and returns location in word string
  returns -1 if char not found

  param char *word: word to be searched
  param char ch: char to search word for
  param int numTimesPrevGuessed: the number of times ch has previously been
                                 correctly guessed by user
*/
int searchword(char *word, char ch, int numTimesPrevGuessed){
  int len = strlen(word);
  int count = 0;

  for(int i = 0; i < len; i++){
    if(word[i] == ch){
      count++;

      if(count > numTimesPrevGuessed){
        return i;
      }

    }
  }

  return -1;

}

int main() {
  srand(time(0));

  char *word; //word to be guessed
  int numlines; //number of lines in file
  int line; //random line chosen from file; will contain word to be guessed
  FILE *wordfile = fopen("words.txt", "r"); //file to be read

  if(wordfile == NULL){

    printf("Error: unable to open file %s\n", "words.txt");
    exit(1);

  }

  word = malloc(sizeof(char) * 32); //allocate word to heap

  if(word == NULL){

    printf("MALLOC ERROR: trouble allocating...");
    exit(1);

  }

  fscanf(wordfile, "%d", &numlines); //read the first line for numlines

  line = rand() % numlines; //pick random line

  for(int i = 0; i < line; i++){

    fscanf(wordfile, "%s", word);

  }

  int len = strlen(word); //number of chars in word

  int isguessed = 0; //boolean. isguessed = 1 when word is guessed

  int num_chars_guessed = 0; //number of chars guessed by user
  //when num_chars_guessed == len, guessing loop ends; word is now guessed

  int count = 0; //total number of guesses to guess word

  //displays string of "_ _ _" depending on how many chars in word
  char *display_guessed_chars;
  display_guessed_chars = malloc(sizeof(char) * len * 2);

  if(display_guessed_chars == NULL){
    
    printf("MALLOC ERROR: trouble allocating...");
    exit(1);

  }

  display_guessed_chars[0] = '_';

  //filling display_guessed_chars with appropriate number of underscores
  for(int i = 1; i < len * 2; i++){

    if(i % 2 == 0 && i <= (len * 2) - 1){

      display_guessed_chars[i] = '_';

    }else if(i >= (len * 2) - 1){

      display_guessed_chars[i] = '\0';

    }else{

      display_guessed_chars[i] = ' ';

    }
  }

  //guessing loop
  while(isguessed == 0){
    char ch; //char guessed by user, updated by scanf each loop
    char parachar; //secondary char to hold \n entered by user in scanf
    count++; //to keep track of number of guesses
    int loc; //location of char guessed by user

    int numTimesPrevGuessed; //if word has been found before

    //prints blank spaces and guessed letters
    printf("\n%s", display_guessed_chars); 

    printf("\nGuess a character: ");
    scanf("%c", &ch); //ch stores guessed char

    scanf("%c", &parachar); //parachar stores '\0' from entering answer
    //this prevents skipped loops where ch holds '\0' and user can't enter
    //new input

    //stores number of times ch has already appeared in word
    numTimesPrevGuessed = timesGuessed(display_guessed_chars, len * 2, ch);

    //location of guessed letter; is -1 if not in word
    loc = searchword(word, ch, numTimesPrevGuessed);

    if(loc >= 0){

      printf("\nSuccess! There is, in fact, a %c in this word", ch);
      display_guessed_chars[loc * 2] = ch; //updates displayed guesses
      num_chars_guessed++; //updates number of correctly guessed chars

    }else if(loc < 0){

      printf("\nNope! There is no %c in this word", ch);

    }

    printf("\n");

    //if all letters have been guessed, end while loop
    if(num_chars_guessed == len){

      isguessed = 1;

    }
  }
  
  //print completed word display
  printf("%s", display_guessed_chars);
  printf("\nYou won in %d turns!\n\n", count);

  //free malloc space
  free(word);
  word = NULL;
  free(display_guessed_chars);
  display_guessed_chars = NULL;

  //close file
  fclose(wordfile);
}


