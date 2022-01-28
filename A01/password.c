/*
 * Takes a password and converts it to a bad password
 * by replacing certain letters with numbers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char *pass = NULL;
	int length;

	pass = malloc(sizeof(char) * (60));
	if(pass == NULL){
		printf("ERROR: malloc overflow. EXITING...");
		exit(1);
	}

	pass = fgets(pass, 60 * sizeof(char), stdin);
	length = strlen(pass);

	printf("You have entered %s as your password\n", pass);

	for(int i = 0; i < length; i++){
		if(pass[i] == 'e' || pass[i] == 'E'){
			pass[i] = '3';
		}else if(pass[i] == 'l' || pass[i] == 'L'){
			pass[i] = '1';
		}else if(pass[i] == 'o' || pass[i] == 'O'){
			pass[i] = '0';
		}else if(pass[i] == 'a' || pass[i] == 'A'){
			pass[i] = '@';
		}
	}
	printf("Your new password is: %s\n", pass);

	free(pass);
	pass = NULL;

	return 0;
}
