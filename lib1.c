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

if(fp == NULL){
    printf("File cannot opened.\n");
    return;
}

char line[500];

while(fgets(line , sizeof(line) , fp)){
    int index;
    char** tokens = split(line , &index);

    //Variable Declaration
    if(0 < index && !strcmp(tokens[0],"int")){

        //When there is no name for the variable, print an error.
        if(1 < index && tokens[1] == NULL){
            fprintf(stderr, "%s", "You need to give a name for the variable.\n");
            exit(-1);
        }
        //When the variable name is not appropriate.
        else if(1 < index && isAppropriateVariableName(tokens[1])){
            fprintf(stderr, "%s", "You need to give an appropriate name for the variable. It cannot contain numbers.\n");
            exit(-1);
        }
        //When the variable name is the special string.
        else if(1 < index && isSpecialKeyword(tokens[1])){
            fprintf(stderr, "%s", "You need to give an appropriate name for the variable. It cannot be same as special keywords.\n");
            exit(-1);
        }
        //When the variable name is the appropriate
        else{
            strcpy(tokens[1],var[var_index].name);
            var[var_index++].value = 0;
        }

        if(2 < index && strcmp(tokens[2],".")){
          fprintf(stderr, "%s", "You need to put END OF LINE (\".\") character at the end of the each line.\n");
          exit(-1);
        }

    }
    //Move Operation
    else if(0 < index && !strcmp(tokens[0],"move")){

    }
    else if(0 < index && !strcmp(tokens[0],"sub")){

    }
    else if(0 < index && !strcmp(tokens[0],"out")){

    }
    else if(0 < index && !strcmp(tokens[0],"add")){

    }
    else if(0 < index && !strcmp(tokens[0],"loop")){

    }
    else{
        fprintf(stderr, "%s", "There is no keyword.\n");
          exit(-1);
    }








}





}
