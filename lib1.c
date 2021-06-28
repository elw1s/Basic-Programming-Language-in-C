#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib1.h"

int STRING_NUMBER = 10 ;
int STRING_LENGTH = 100;
variable var[1000];
int var_index = 0;


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

	return tokens;
}

void parse(char* file_name){

FILE *fp = fopen(file_name , "r");

if(f == NULL){
    printf("File cannot opened.\n");
    return;
}

char line[500];

while(fgets(line , sizeof(line) , fp)){
    int index;
    char** tokens = split(line , &index);

    //Varible Declaration
    if(0 < index && !strcmp(tokens[0],"int")){
        if(1 < index && )





    }








}





}
