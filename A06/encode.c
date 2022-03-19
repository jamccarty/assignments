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

unsigned char store_bit(unsigned char ch, unsigned char px){
  if(ch == 0x0){
    ch = ~ch - 1;
    return ch & px;
  }
  return ch | px;
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

  unsigned char mes_masks[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

  int hcount = 0; 
  int wcount = 0;
  int icount = 0;
  int charcount = 0;
  int shift_mask = 7;

  while(charcount != len){
    unsigned char bit = message[charcount] & mes_masks[7 - shift_mask];

    bit = bit >> shift_mask;

    pxs[hcount][wcount].colors[icount] = store_bit(bit, 
        pxs[hcount][wcount].colors[icount]);

    icount++;
    shift_mask--;

    if(icount == 3){
      icount = 0;
      wcount++;
    }

    if(wcount == w){
      wcount = 0;
      hcount++;
    }

    if(shift_mask == -1){
      shift_mask = 7;
      charcount++;
    }
  }

  for(int i = 0; i < 8; i++){
    if(icount == 3){
      icount = 0;
      wcount++;
    }
    if(wcount == w){
      wcount = 0;
      hcount++;
    }
    if(hcount == h){
      break;
    }

    unsigned char bit = '\0' & mes_masks[i];
    pxs[hcount][wcount].colors[icount] = store_bit(bit,
        pxs[hcount][wcount].colors[icount]);
    icount++;
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

  for(int i = 0; i < h; i++){
    free(pxs[i]);
    pxs[i] = NULL;
  }
  free(pxs);

  free(message);
  message = NULL;
}

