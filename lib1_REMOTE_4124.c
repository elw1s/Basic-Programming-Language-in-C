#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib1.h"

int STRING_NUMBER = 10 ;
int STRING_LENGTH = 100;
variable var[1000];
int var_index = 0;
int error_line = 0;
int bracket = 0;


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

char** split_NEWLINE(char *line , int * size ){

char **tokens;

	*size = 0;
	tokens = malloc(sizeof(char*) * STRING_NUMBER);

	for(int i = 0; i < STRING_NUMBER; i++){
		tokens[i] = malloc(sizeof(char) * STRING_LENGTH);
	}

	tokens[*size] = strtok(line , ".");

	while(tokens[*size] != NULL){

		++(*size);
		if(*size > STRING_NUMBER - 20){
			STRING_NUMBER *= 5;
			tokens = realloc(tokens , sizeof(char*) * STRING_NUMBER);
		}

		tokens[*size] = strtok(NULL , ".");
	}
	return tokens;
}




void parse(char* file_name){

	FILE *fp = fopen(file_name , "r");

	if(fp == NULL){
	    printf("File cannot opened.\n");
	    return;
	}

	/*char *codes[500] , *temp , line[500];
	int code_count = 0;

	while(fgets(line , sizeof(line) , fp)){

		int r = 0 , c = 0;
		for(int i = 0 ; i< strlen(line); i++){

			if( !(line[i] == '.' || line[i] == '\n' || line[i] == '\r')){
				codes[r][c++] = line[i];
			}
			else{
				r++;
				code_count++;
			}
		}
	}

	for(int i = 0; i< code_count ; i++){
		printf("%s\n",codes[i]);
	}
	printf("%s\n",codes[0]);
	printf("%s\n",codes[1]);
	printf("%s\n",codes[2]);

	fclose(fp);*/

	char line[500];
	while(fgets(line , sizeof(line) , fp)){
	    int size;
	    error_line++;
	    char** tokens = split(line , &size);


	    //Variable Declaration
	    if(0 < size && !strcmp(tokens[0],"int")){

	        //When there is no name for the variable, print an error.
	        if(1 < size && tokens[1] == NULL){
	            fprintf(stderr, "%s", "You need to give a name for the variable.\n");
	            exit(-1);
	        }
	        //When the variable name is not appropriate.
	        else if(1 < size && isVariable(tokens[1])){
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

	    	// Parse with ','

	    	for(int i = 0; i< strlen(tokens[1]) ; i++){
	    		if(tokens[1][i] != '"'){
	    			tokens[1] = tokens[1] + 1;

	    			for(int j = 0 ; j< strlen(tokens[1]) ; j++){
	    				if(tokens[1][j] == '"'){
	    					tokens[1][j] = '\0';
	    					break;
	    				}
	    			}

	    			printf("%s\n",tokens[1]);
	    			break;
	    		}
	    	}

	    }
	    else if(0 < size && !strcmp(tokens[0],"add")){

	    }
	    else if(0 < size && !strcmp(tokens[0],"loop")){
             if(1 < size && tokens[1] == NULL){
                printf("ERROR at line %d\n",error_line);
	            fprintf(stderr, "%s", "You need to specify the number of turns of the loop.\n");
	            exit(-1);
	        }
	        else if(2 < size && strcmp(tokens[2],"times") != 0){
                printf("ERROR at line %d\n",error_line);
	            fprintf(stderr, "%s", "Missing keyword: \"times\".\n");
	            exit(-1);
	        }

	    }
	    else{
	        fprintf(stderr, "%s", "There is no keyword.\n");
	          exit(-1);
	    }
	}

	fclose(fp);

}

int isThereOpenBracket(char * line , int str_len){
    for(int i = 0; i < str_len; i++){
        if(line[i] == '[') return 1;
    }
    return 0;
}

int isThereCloseBracket(char * line , int str_len){
    for(int i = 0; i < str_len; i++){
        if(line[i] == ']') return 1;
    }
    return 0;
}

int isSpecialKeyword(char *str){
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
        if(i == 0 && str[i] == '_') return 0;
		else if(!(isalnum(str[i])){
			return 0;
		}
	}
	return 1;
}

