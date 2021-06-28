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

	    //Varible Declaration
	    if(0 < size && !strcmp(tokens[0],"int")){
	        if(1 < size ){

			}
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

