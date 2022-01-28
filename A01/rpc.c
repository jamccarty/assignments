/* 
 * Allows player to play x amount of Rock, Paper, Scissors
 * with an AI, with player choosing x.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

int main() {
  srand(time(0));
  int uwins = 0; //number of games user has won
  int cwins = 0; //number of games AI has won
  int numplay = 0; //number of games they will play

  int random_AI_name = rand() % 4;
  char AI_name[20];

  //chooses a random AI name for program to personify AI with
  if(random_AI_name == 0){
    strcpy(AI_name, "Nancy");
  }else if(random_AI_name == 1){
    strcpy(AI_name, "Jerome");
  }else if(random_AI_name == 2){
    strcpy(AI_name, "Eggbert");
  }else{
    strcpy(AI_name, "Pope Francis 2.0");
  }

  /*
   * columns and rows are arranged rock, paper, scissors
   * so win_chart[0][1] would be rock vs paper, and paper wins
   * if the row (AI) "player" wins, there will be a 1 in the matrix
   * if the column (user) "player" does, there will be a 2
   * if there is no winner, there will be a 0
   * The last column is for when the user enters an invalid entry
   * if the user's entry is invalid, it will be -1
   */
                      //  R     P     S   Invalid
  int win_chart[3][4] = {{0,    1,    2,    -1}, //Rock     (row 0) 
                         {2,    0,    1,    -1}, //Paper    (row 1)
                         {1,    2,    0,    -1}  //Scissors (row 2)
                        };

  printf("\nWelcome to Rock, Paper, Scissors!\n");
  printf("Today, you will be playing against our lovely AI, %s\n", AI_name);
  printf("How many rounds do you want to play? ");
  scanf("%d", &numplay);

  //if invalid answer, exits
  if(numplay <= 0){
    printf("\n%s thinks you are very rude, and shouldn't ask to play Rock, Paper,\n", AI_name);
    printf("Scissors if you don't actually want to play, but whatever\n\n");
    printf("EXITING...\n\n");
    return 1;
  }

  for(int i = 0; i < numplay; i++){
    char uchoice[25]; //user/player's choice: "rock", "paper", "scissors"
    char cchoice[25]; //computer/AI's choice: "rock", "paper", "scissors"

    char AI_wins_verb[25]; //i.e. if rock wins, rock "smashes" scissors
    char AI_loses_verb[25]; //same as above, but for if AI loses

    int cplay; //AI choice number designation (0 = r, 1 = p, 2 = s)
    int uplay; //player choice number designation

    printf("\nWhich do you choose? rock, paper, or scissors? ");
    scanf("%s", uchoice);
    int len = strlen(uchoice); //length of user's choice
    
    //converts user's choice to lowercase for easy comparison
    for(int i = 0; i < len; i++){
      uchoice[i] = tolower(uchoice[i]);
    }

    //if cplay = 0, computer chooses rock
    //if cplay = 1, computer chooses paper
    //if cplay = 2, computer chooses scissors
    cplay = rand() % 3;

    if(cplay == 0){
      strcpy(cchoice, "rock");
      strcpy(AI_wins_verb, "smashes");
      strcpy(AI_loses_verb, "covers");
    }else if(cplay == 1){
      strcpy(cchoice, "paper");
      strcpy(AI_wins_verb, "covers");
      strcpy(AI_loses_verb, "cuts");
    }else{
      strcpy(cchoice, "scissors");
      strcpy(AI_wins_verb, "cuts");
      strcpy(AI_loses_verb, "smashes");
    }

    //initializes uplay
    if(strcmp(uchoice, "rock") == 0){
      uplay = 0;
    }else if(strcmp(uchoice, "paper") == 0){
      uplay = 1;
    }else if(strcmp(uchoice, "scissors") == 0){
      uplay = 2;
    }else{
      uplay = 3;
    }

    //print AI choice
    printf("\t%s chose %s\n", AI_name, cchoice);

    //prints different results based on who wins
    if(win_chart[cplay][uplay] == 0){
      printf("\tSometimes in life, there are no winners\n");
    }else if(win_chart[cplay][uplay] == 1){
      printf("\t%s %s %s\n", uchoice, AI_loses_verb, cchoice);
      uwins++;
    }else if(win_chart[cplay][uplay] == 2){
      printf("\t%s %s %s\n", cchoice, AI_wins_verb, uchoice);
      cwins++;
    }else if(win_chart[cplay][uplay] == -1){
      printf("\tYou entered an invalid choice: %s\n", uchoice);
    }


    printf("%s's score: %d, Your score: %d\n", AI_name, cwins, uwins);
  }

  //prints winner
  if(uwins > cwins){
    printf("\nYou have bested %s!\n\n", AI_name);
  }else if(cwins > uwins){
    printf("%s wins!\n", AI_name);
  }else{
    printf("It's a tie!\n");
  }
  return 0;
}

