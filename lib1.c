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
FILE *fp;


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


void read(char *file_name){
fp = fopen(file_name , "r");
	if(fp == NULL){
	    printf("File cannot opened.\n");
	    return;
	}
	char line[500];
	while(fgets(line , sizeof(line) , fp)){
        parse(line);
	}
fclose(fp);
}


void parse(char *line){

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
            for(int i = 0; i< strlen(tokens[1]) ; i++){
                if(tokens[1][i] == '.'){
                    tokens[1][i] = '\0';
                }
            }

            strcpy(var[var_index].name , tokens[1]);
            var[var_index++].value = 0;
        }

        if(2 < size && strcmp(tokens[2],".")){
          fprintf(stderr, "%s", "You need to put END OF LINE (\".\") character at the end of the each line.\n");
          exit(-1);
        }
    }
    //Move Operation
    else if(0 < size && !strcmp(tokens[0],"move")){

        long int to_be_moved;

        // Source
        if(size != 4){
            fprintf(stderr, "%s", "move (A source) to ( A destination) expected.\n");
            exit(-1);
        }

        if(isSpecialKeyword(tokens[1])){
            fprintf(stderr, "%s", "Source cannot be a keyword.\n");
            exit(-1);
        }
        else if(isInt(tokens[1] , strlen(tokens[1]))){
            to_be_moved = atol(tokens[1]);
        }
        else if(isVariable(tokens[1])){

            to_be_moved = getValue(tokens[1]);
        }
        else{
            fprintf(stderr, "%s", "Source is not known.\n");
            exit(-1);
        }

        //To
        if(strcmp(tokens[2] , "to") != 0){
            fprintf(stderr, "%s", "move (A source) to ( A destination) expected.\n");
            exit(-1);
        }

        // Destination

        for(int i= 0 ;i < strlen(tokens[3]); i++){ // Modifying the integer to get rid of the '\n'
            if(tokens[3][i] == '.'){
                tokens[3][i] = '\0';
                break;
            }
        }

        if(isSpecialKeyword(tokens[3])){
            fprintf(stderr, "%s", "Destination cannot be a keyword.\n");
            exit(-1);
        }
        else if(isInt(tokens[3] , strlen(tokens[3]))){
            fprintf(stderr, "%s", "Destination cannot be an integer.\n");
            exit(-1);
        }
        else if(isVariable(tokens[3])){

            for(int i = 0; i< var_index; i++){
                if(strcmp( var[i].name , tokens[3]) == 0){
                    var[i].value = to_be_moved;
                    break;
                }
            }
        }
        else{
            fprintf(stderr, "%s", "Destination is not known.\n");
            exit(-1);
        }


    }
    else if(0 < size && !strcmp(tokens[0],"sub")){

        long int to_be_subbed;

        if(size != 4){
            fprintf(stderr, "%s", "sub <int_value> from <variable>. expected.\n");
            exit(-1);
        }

        long int to_be_added;

        // The value to be substracted:
        if(isSpecialKeyword(tokens[1])){        
            fprintf(stderr, "%s", "sub <int_value> from <variable>. expected.\n");
            exit(-1);
        }
        else if(isInt(tokens[1] , strlen(tokens[1]))){
            to_be_subbed = atol(tokens[1]);
        }
        else if(isVariable(tokens[1])){

            to_be_subbed = getValue(tokens[1]);
        }
        else{
            fprintf(stderr, "%s", "sub <int_value> from <variable>. expected.\n");
            exit(-1);
        }

        //From
        if(strcmp(tokens[2] , "from")){ // third argument (second index) is not "from".
            fprintf(stderr, "%s", "sub <int_value> from <variable>. expected.\n");
            exit(-1);
        }

        //Variable

        for(int i = 0; i< strlen(tokens[3]) ; i++){ // Get rid of '.'
            if(tokens[3][i] == '.'){
                tokens[3][i] = '\0';
                break;
            }
        }

        if(!isVariable(tokens[3])){ // fourth argument is not a variable.
            fprintf(stderr, "%s", "sub <int_value> from <variable>. expected.\n");
            exit(-1);
        }

        for(int i = 0; i< var_index; i++){
            if(strcmp(var[i].name , tokens[3]) == 0){
                var[i].value -= to_be_subbed;
                break;
            }
        }


    }
    else if(0 < size && !strcmp(tokens[0],"out")){

        for(int i = 1; i< size; i++){

            for(int j = 0; j< strlen(tokens[i]) ; j++){ // Get rid of ','
                if(tokens[i][j] == ','){
                    tokens[i][j] = '\0';
                    break;
                }
            }

            if( i == size - 1){ // Get rid of the '.' dot at the end.
                for(int j = 0; j< strlen(tokens[i]); j++){
                    if(tokens[i][j] == '.'){
                        tokens[i][j] = '\0';
                    }
                }
            }

            if(strstr( tokens[i] , "\"") ){

            	int flag = 1; // flag is 0 if the string doesn't contain spaces.
                for(int k = 0; k< strlen(tokens[i]) ; k++){

                    if(tokens[i][k] != '"'){
                        tokens[i] = tokens[i] + 1;

                        for(int j = 0 ; j< strlen(tokens[i]) ; j++){
                            if(tokens[i][j] == '"'){
                                tokens[i][j] = '\0';
                                flag = 0;
                                break;
                            }
                        }

                        printf("%s",tokens[i]);
                        break;
                    }
                }
                if(flag == 1){ // There are spaces in the string.
                	i++;
                	while(!strstr(tokens[i] , "\"")){             		
                        printf(" %s",tokens[i]);
                        i++;
                        if(i >= size){
                            fprintf(stderr, "%s", "Quatation mark never closed.\n");
                            exit(-1);
                        }
                	}

                    // The part where the quatation mark will be closed.
                    for(int j = 0 ; j< strlen(tokens[i]) ; j++){
                        if(tokens[i][j] == '"'){
                            tokens[i][j] = '\0';
                            flag = 0;
                            break;
                        }
                    }

                    printf(" %s",tokens[i]);

                	
                } 
            }
            else if(strstr( tokens[i] , "newline")){
                printf("\n");
            }
            else if(isVariable(tokens[i])){
                printf("%ld" , getValue(tokens[i]));
            }
        }



    }
    //Add Operation
    else if(0 < size && !strcmp(tokens[0],"add")){



    	if(size != 4){
    		fprintf(stderr, "%s", "add <int_value> to <variable>. expected.\n");
            exit(-1);
    	}

    	long int to_be_added;

    	// will be added value:
    	if(isSpecialKeyword(tokens[1])){		
    		fprintf(stderr, "%s", "add <int_value> to <variable>. expected.\n");
            exit(-1);
    	}
    	else if(isInt(tokens[1] , strlen(tokens[1]))){
            to_be_added = atol(tokens[1]);
        }
        else if(isVariable(tokens[1])){

        	to_be_added = getValue(tokens[1]);
        }
        else{
        	fprintf(stderr, "%s", "add <int_value> to <variable>. expected.\n");
            exit(-1);
        }

        //To
        if(strcmp(tokens[2] , "to")){ // third argument (second index) is not "to".
        	fprintf(stderr, "%s", "add <int_value> TO <variable>. expected.\n");
            exit(-1);
        }

        //Variable

        for(int i = 0; i< strlen(tokens[3]) ; i++){ // Get rid of '.'
        	if(tokens[3][i] == '.'){
        		tokens[3][i] = '\0';
        		break;
        	}
        }

        if(!isVariable(tokens[3])){ // fourth argument is not a variable.
        	fprintf(stderr, "%s", "add <int_value> to <variable>. expected.\n");
            exit(-1);
        }

        for(int i = 0; i< var_index; i++){
        	if(strcmp(var[i].name , tokens[3]) == 0){
        		var[i].value += to_be_added;
        		break;
        	}
        }



    }
    //Loop
    else if(0 < size && !strcmp(tokens[0],"loop")){
         long int loop_times = 0;
         if(1 < size && tokens[1] == NULL){
            printf("ERROR at line %d\n",error_line);
            fprintf(stderr, "%s", "You need to specify the number of turns of the loop.\n");
            exit(-1);
        }
        else if(1 < size && isVariable(tokens[1])){
            loop_times = getValue(tokens[1]);
            setValue(tokens[1],0);
        }
        else if(1 < size && isInt(tokens[1], strlen(tokens[1]))){
            loop_times = atol(tokens[1]);
        }
        else if(2 < size && strcmp(tokens[2],"times") != 0){
            printf("ERROR at line %d\n",error_line);
            fprintf(stderr, "%s", "Missing keyword: \"times\".\n");
            exit(-1);
        }
        if (3 < size && !strcmp(tokens[3],"out")){
                int print_newline = 0;

                if(size < 4){
                    printf("ERROR at line %d\n",error_line);
                    fprintf(stderr, "%s", "Missing String Literal: \"Example\".\n");
                    exit(-1);
                }
                else if(5 < size && strcmp(tokens[5],",") != 0){
                    printf("ERROR at line %d\n",error_line);
                    fprintf(stderr, "%s", "Missing Comma:  , \n");
                    exit(-1);
                }
                else if(6 < size && strcmp(tokens[6],"newline")){
                    printf("ERROR at line %d\n",error_line);
                    fprintf(stderr, "%s", "Missing Newline: newline\n");
                    exit(-1);
                }

                if(6 < size && !strcmp(tokens[6],"newline")) print_newline = 1;
                char * str = remove_literal(tokens[4]);
                for(int i = 0; i < loop_times; i++){
                    printf("%s", str);
                    if(print_newline) printf("\n");
                }

        }
        else{
            char ** newCode;
            int newCode_line = 0;
            while(fgets(line , sizeof(line) , fp)){
                tokens = split(line , &size);

                newCode_line++;
                if(isThereOpenBracket(tokens[0],strlen(tokens[0]))){
                   bracket = 1;
                }
                if(bracket == 1){

                }
                else{

                }
            }
        }
    }
    else if(isThereOpenBracket(line , strlen(line)) || isThereCloseBracket(line , strlen(line))){

    }
    else{
        fprintf(stderr, "%s", "There is no keyword.\n");
          exit(-1);
    }

}

char* remove_literal(char * line){
char * arr;
char * token = strtok(line , "\"");

while(token != NULL){
    strcat(arr , token);
    token = strtok(NULL , "\"");
}
return arr;
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
		|| !strcmp(str , "move") )
	{
		return 1;
	}
	return 0;
}
int isVariable(char *str){
	// Does this variable name exist ?
	for(int i = 0; i< var_index; i++){
		if( !strcmp(var[i].name , str ))
			return 1;
	}
	return 0;
}
int isAppropriateVariableName(char *str){

	int length = strlen(str);

	for(int i = 0; i< length; i++){

        if(i == 0 && str[i] == '_') return 0;

		else if(!(isalnum(str[i]))){
			return 0;
		}
	}
	return 1;
}
long int getValue(char *str){
	for(int i = 0; i< var_index; i++){
		if( !strcmp(var[i].name , str ))
			return var[i].value;
	}
	return -1;
}

void setValue(char *str , int value){
    for(int i = 0; i< var_index; i++){
		if( !strcmp(var[i].name , str ))
			var[i].value = value;
			return;
	}
}

int isInt(char * str , int size){
    for(int i = 0; i< size; i++){
        if(!isdigit(str[i])) return 0;
    }
    return 1;
}

