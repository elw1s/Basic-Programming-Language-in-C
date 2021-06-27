#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib1.h"

int STRING_NUMBER = 10 ;
int STRING_LENGTH = 100;
// Remember to free
char** split( char *line , int *index){

	char **tokens;
	*index = 0;
	tokens = malloc(sizeof(char*) * STRING_NUMBER);

	for(int i = 0; i < STRING_NUMBER; i++){
		tokens[i] = malloc(sizeof(char) * STRING_LENGTH);
	}

	tokens[*index] = strtok(line , " ");

	while(tokens[*index] != NULL){

		++(*index);
		if(*index > STRING_NUMBER - 20){
			STRING_NUMBER *= 5;
			tokens = realloc(tokens , sizeof(char*) * STRING_NUMBER);
		}

		tokens[*index] = strtok(NULL , " ");
	}
	(*index)--;
	return tokens;
}