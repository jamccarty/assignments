//Encodes a message in a binary .ppm image file by making the least
//significant bit of each pixel color correspond to a bit in a char
//all chars eventually form message
//
//expected compilation: make encode
// ^^use attached Makefile
//expected run: 
//              ./encode <sourcefile.ppm> <destinationfile.ppm>
//and enter message by hand
//or:           ./encode <sourcefile.ppm> <destinationfile.ppm> < <msg.txt>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "read_ppm.h"

//returns least significant bit in binary form of unsigned char
unsigned char leastBit(unsigned char ch){
  return '0' + (ch % 2);
}

//returns 8-bit binary form of single unsigned character
//as array of unsigned characters where bin[0] is most significant
//bit and bin[7] is the least significant
unsigned char* encode(unsigned char msg){
  unsigned char *bin = malloc(sizeof(unsigned char) * 8);
  int b = 7;
  for(int i = 0; i < 8; i++){
    if(msg % 2 == 1){
      bin[b] = '1';
    }else{
      bin[b] = '0';
    }
    b--;
    msg = msg / 2;
  }
  return bin;
}

int main(int argc, char** argv) {
  if(argc <= 1){
    printf("ERROR: program must be run ./a.out <source file> <dest file>\n");
    printf("NOTE: only <source file> is mandatory for implementation\n");
    exit(1);
  }

  char *message; //message to be encoded in new file
  int mes_len = 0; //length of message
  int msg_numlines = 1; //number of lines in message
  char *linemsg; //single line in message; recycled

  int w = 0; //width of ppm
  int h = 0; //height of ppm

  //holds pixel information
  struct ppm_pixel** pxs = read_ppm(argv[1], &w, &h);

  mes_len = (w * h * 3)/8; //maximum length of message (to be malloced)
  message = malloc(sizeof(char) * mes_len + sizeof(char));
  memset(message, '\0', ((w * h * 3)/8) + 1);
  printf("How many lines does your message contain: ");
  scanf("%d", &msg_numlines);//setting number of lines
  
  char holdspace; //holds additional space
  scanf("%c", &holdspace);

  printf("Please enter the message you wish to encode: \n");
  for(int i = 0; i < msg_numlines; i++){
    printf("Line %d: ", i);
    linemsg = malloc(sizeof(char) * 82);
    memset(linemsg, '\0', 82);
    fgets(linemsg, 82, stdin); //getting individual line

    strcat(message, linemsg); //appending line to message
    free(linemsg);
  }
  int len = strlen(message); //len is now actual length of message

  //array of arrays. each unsigned char* array is the 8-bit binary form
  //of each character in message
  unsigned char **binmsg = malloc(sizeof(unsigned char*) * (len + 1));

  //encoding each bit in message
  for(int i = 0; i < len; i++){
    binmsg[i] = encode(message[i]);
  }
  //encoding end of string
  binmsg[len] = encode('\0');

  int loc = 0; //location within the binmsg array
  int e = 0; //location within each char byte in binmsg array
  
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      for(int k = 0; k < 3; k++){
        if(loc < (len + 1)){
          //ch is least significant bit in current 8-bit color
          char ch = leastBit(pxs[i][j].colors[k]);
          //if it doesn't already equal the next bit in binmsg
          if(ch != binmsg[loc][e]){
            if(pxs[i][j].colors[k] < 255){
              pxs[i][j].colors[k]++;
            }else{
              //if ch=255, subtracts 1 to prevent overflow
              pxs[i][j].colors[k]--;
            }
          }
        }
        e++; //move to next bit
        //if at last bit, move to next "char" in binmsg
        if(e == 8){
          loc++;
          e = 0;
        }
      }
    }
  }
  //length of source file name
  int fl = strlen(argv[1]);
  char *newfile;
  //if there is no inputted destination file,
  //make newfile = <filename>-encoded.ppm
  if(argc < 3){
    newfile = malloc(sizeof(char) * (fl + 9));
    newfile[0] = '\0';
    for(int i = fl - 1; i >= fl - 4; i--){
      newfile[i] = '\0';
    }
    strcat(newfile, "-encoded.ppm");
    write_ppm(newfile, pxs, w, h);
  }else{
    //if there is an inputted destination file,
    //write encoded ppm to that
    int dlen = strlen(argv[2]);
    newfile = malloc(sizeof(char) * (dlen + 1));
    write_ppm(argv[2], pxs, w, h);
  }

  //free all memory
  free(newfile);
  newfile = NULL;
  for(int i = 0; i <= len; i++){
    free(binmsg[i]);
    binmsg[i] = NULL;
  }
  free(binmsg);
  binmsg = NULL;

  for(int i = 0; i < h; i++){
    free(pxs[i]);
    pxs[i] = NULL;
  }
  free(pxs);

  free(message);
  message = NULL;
}

