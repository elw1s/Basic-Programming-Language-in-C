#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib1.h"

// add , out , newline , sub , loop , int , move

void main(){

	char line[150];

	FILE *f = fopen("x.ba","r");
	// Openning file.
	if(f == NULL){
		printf("File cannot opened.\n");
		return;
	}

	fgets(line , 150 , f);
	fgets(line , 150 , f);
	fgets(line , 150 , f);

	int index , i = 0;
	char **bum = split(line , &index);
	
	while(bum[i] != NULL){
		printf("%s\n",bum[i]);
		i++;
	}

	fclose(f);
}

