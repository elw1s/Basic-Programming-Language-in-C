#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib1.h"

int STRING_NUMBER = 10 ;
int STRING_LENGTH = 100;
variable var[1000];
int var_index = 0;


// Remember to free
char** split( char *line , int *size){

	char **tokens;
	*size = 0;
	tokens = malloc(sizeof(char*) * STRING_NUMBER);

	for(int i = 0; i < STRING_NUMBER; i++){
		tokens[i] = malloc(sizeof(char) * STRING_LENGTH);
	}

	tokens[*size] = strtok(line , " ");

	while(tokens[*size] != NULL){

		++(*size);
		if(*size > STRING_NUMBER - 20){
			STRING_NUMBER *= 5;
			tokens = realloc(tokens , sizeof(char*) * STRING_NUMBER);
		}

		tokens[*size] = strtok(NULL , " ");
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
    int size;
    char** tokens = split(line , &size);

    //Variable Declaration
    if(0 < size && !strcmp(tokens[0],"int")){

        //When there is no name for the variable, print an error.
        if(1 < size && tokens[1] == NULL){
            fprintf(stderr, "%s", "You need to give a name for the variable.\n");
            exit(-1);
        }
        //When the variable name is not appropriate.
        else if(1 < size && isAppropriateVariableName(tokens[1])){
            fprintf(stderr, "%s", "You need to give an appropriate name for the variable. It cannot contain numbers.\n");
            exit(-1);
        }
        //When the variable name is the special string.
        else if(1 < size && isSpecialKeyword(tokens[1])){
            fprintf(stderr, "%s", "You need to give an appropriate name for the variable. It cannot be same as special keywords.\n");
            exit(-1);
        }
        //When the variable name is the appropriate
        else{
            strcpy(tokens[1],var[var_index].name);
            var[var_index++].value = 0;
        }

        if(2 < size && strcmp(tokens[2],".")){
          fprintf(stderr, "%s", "You need to put END OF LINE (\".\") character at the end of the each line.\n");
          exit(-1);
        }

    }
    //Move Operation
    else if(0 < size && !strcmp(tokens[0],"move")){

    }
    else if(0 < size && !strcmp(tokens[0],"sub")){

    }
    else if(0 < size && !strcmp(tokens[0],"out")){

    }
    else if(0 < size && !strcmp(tokens[0],"add")){

    }
    else if(0 < size && !strcmp(tokens[0],"loop")){

    }
    else{
        fprintf(stderr, "%s", "There is no keyword.\n");
          exit(-1);
    }
}

}

int isKeyword(char *str){
	// add , out , newline , sub , loop , int , move , from

	if( !strcmp(str , "add") || !strcmp(str , "out") || !strcmp(str , "newline") ||
		!strcmp(str , "sub") || !strcmp(str , "loop") || !strcmp(str , "int")
		|| !strcmp(str , "move") || !strcmp(str , "from") )
	{
		return 1;
	}
	return 0;
}
int isAppropriateVariableName(char *str){
	// Does this variable name exist ?
	for(int i = 0; i< var_index; i++){
		if( !strcmp(var[i].name , str ))
			return 0;
	}
	return 1;
}
int isVariable(char *str){

	int length = strlen(str);

	for(int i = 0; i< length; i++){
		if( !(isalnum(str[i]) || str[i] == '_')){
			return 0;
		}
	}
	return 1;
}

