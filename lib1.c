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
int comma = 0;
char *code;
int num_lines;
FILE *fp;


// Remember to free
char** split( char *line , char * delimiter, int *size){

	char **tokens;
	*size = 0;
	tokens = malloc(sizeof(char*) * STRING_NUMBER);

	for(int i = 0; i < STRING_NUMBER; i++){
		tokens[i] = malloc(sizeof(char) * STRING_LENGTH);
	}

	tokens[*size] = strtok(line , delimiter);

	while(tokens[*size] != NULL){

		++(*size);
		if(*size > STRING_NUMBER - 20){
			STRING_NUMBER *= 5;
			tokens = realloc(tokens , sizeof(char*) * STRING_NUMBER);
		}

		tokens[*size] = strtok(NULL , delimiter);
	}

	return tokens;
}

void read(char *file_name){
	long lSize;

	fp = fopen(file_name, "rb");

	if(fp == NULL){
	    printf("File cannot opened.\n");
	    return;
	}

	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	code = calloc(1, lSize + 1);
	if (!code) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

	if (1 != fread(code, lSize, 1, fp)) {
		fclose(fp), free(code), fputs("entire read fails", stderr), exit(1);
	}
	removeCommentLines(code);
    char ** lines = split(code , "." , &num_lines);
    for(int i = 0; i < num_lines ; i++){
        parse(lines[i]);
    }
fclose(fp);
}

char left_substr[500];
char right_substr[500];
/* This function removes the comment lines from the given parameter */
void removeCommentLines(char* code) {
	int completed = 0; //This will be true when there is a closing curly bracket
	for (int i = 0; i < strlen(code); i++) { //Search for {
		if (code[i] == '{') { //When you find a curly bracket then look for the closing curly bracket
			for (int j = i; j < strlen(code); j++) {
				if (code[j] == '}') { //When you find the closing curly bracket, then you need to remove it from the code.
					substring(code, 0, i, left_substr); //Firstly, take the left substring of the '{'
					substring(code, j + 1, strlen(code), right_substr); //Secondly, take the right substring of the '}'
					strcat(left_substr, right_substr); //Concentrate these substring into the left_substr variable
					strcpy(code, left_substr); //Afterwards, copy the new code lines (comment is removed) into the code variable
					completed = 1; //Set completed 1 because we have found the '}'
					removeCommentLines(code); //Look for the other comments
				}
			}
			//If there is no '}' but '{', then print an error and exit.
			if (!completed) {
				fprintf(stderr, "%s", "You need to close the comment line.\n");
                exit(-1);
			}
		}
	}
}

//Substring function
int substring(char *source, int from, int n, char *target) {
	int length, i;
	for (length = 0; source[length] != '\0'; length++);
	if (from > length) {
		printf("Starting index is invalid.\n");
		return 1;
	}
	if ((from + n) > length) {
		n = (length - from);
	}
	for (i = 0; i < n; i++) {
		target[i] = source[from + i];
	}
	target[i] = '\0';
	return 0;
}

void parse(char *line){
    int size;
    error_line++;
    char** tokens = split(line , " \n\r" , &size);

     //Variable Declaration
    if(0 < size && !strcmp(tokens[0],"int")){
        //When there is no name for the variable, print an error.
        if(1 < size && tokens[1] == NULL){
            fprintf(stderr, "%s", "You need to give a name for the variable.\n");
            exit(-1);
        }
        //When the variable name is not appropriate.
        else if(1 < size && !isAppropriateVariableName(tokens[1])){
            fprintf(stderr, "%s", "You need to give an appropriate name for the variable. It cannot start with numbers and underscore.\n");
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

        int to_be_moved;

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

            for(int i = 0; i< var_index; i++){
                if(strcmp( var[i].name , tokens[1]) == 0){
                    to_be_moved = var[i].value;
                    break;
                }
            }
        }
        else if(4 < size && strcmp(tokens[4],".")){
          fprintf(stderr, "%s", "You need to put END OF LINE (\".\") character at the end of the each line.\n");
          exit(-1);
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
    /* Errorlarda problem var. Tanýmlý bir variable olmasa bile error vermiyor */
    else if(0 < size && !strcmp(tokens[0],"out")){


        if(1 < size && tokens[1] == NULL){

            fprintf(stderr, "%s", "You need to give a string literal or a variable to print.\n");
            exit(-1);
        }
        int printed = 0;

        for(int i = 1; i < size; i++){
            if(strcmp(tokens[i],",") != 0 && printed == 0){
                if(tokens[i][0] == '"'){
                    for(i; i < size; i++){
                        if(tokens[i][0] == '"' && tokens[i][strlen(tokens[i]) - 1] == '"'){
                            char * temp = remove_literal(tokens[i],0);
                            temp = remove_literal(temp , strlen(temp) - 1);
                            printf("%s",temp);
                            break;
                        }
                        else if(tokens[i][0] == '"'){
                            printf("%s",remove_literal(tokens[i],0));
                        }
                        else if(tokens[i][strlen(tokens[i]) - 1] == '"'){
                            if(strlen(tokens[i]) < 2) printf(" ");
                            else printf(" %s",remove_literal(tokens[i],strlen(tokens[i])-1));
                            break;
                        }
                        else{
                            printf(" %s",tokens[i]);
                        }
                    }
                }
                else if(isVariable(tokens[i])){
                    printf("%ld",getValue(tokens[i]));
                }
                else if(!strcmp(tokens[i],"newline")){
                    printf("\n");
                }
                else{  
                    fprintf(stderr, "%s%d\n", "You need to give a string literal or a variable to print. Error at line " , error_line);
                    exit(-1);
                }
                printed = 1;
            }
            else if(!strcmp(tokens[i],",")){
                printed = 0;
            }
            else{
                fprintf(stderr, "%s%d\n", "Missing comma between string literals or variables. Error at line " , error_line);
                exit(-1);
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
                        if(4 < size){
                            for(int i = 0; i < loop_times; i++){
                                char str[500] = "";
                                for(int i = 3; i < size; i++){
                                    strcat(str , " ");
                                    strcat(str ,tokens[i]);
                                }
                                parse(str);
                            }
                    }
        }
        else if (3 == size && !strcmp(tokens[3],"out")){
            printf("ERROR at line %d\n",error_line);
            fprintf(stderr, "%s", "Missing String Literal: \"Example\".\n");
            exit(-1);
        }
        else{
            char ** newCode = malloc(sizeof(char*)*100); //A for loop can contain 100 Lines within brackets
                for(int i=0;i<100; i++)
                    newCode[i] = malloc(sizeof(char)*5000); //A line can contain 5000 characters.
            int newCode_line = 0;
            int tmp_size = 0;
            char ** temp = split(code , "." , &tmp_size);
            int target = 0;
            int equal;
            printf("TEST1\n");
            for(target; target < num_lines; target++){
                    equal = 1;
                    for(int i = 0; i < size && i < tmp_size; i++){
                        if(strcmp(tokens[i],temp[i]) != 0){
                                printf("Eşit değiller. %s != %s\n",tokens[i],temp[i]);
                            equal = 0;
                        }
                    }
                    if(equal) break;
            }
            printf("TEST2\n");
            printf("TARGET LINE = %d",target);
            for(int i = target; i < num_lines; i++){
                printf("TARGET LINE = %d",target);
                char ** line_code = split(temp[i]," \n\r" , &tmp_size);
                if(isThereOpenBracket(temp[i],strlen(temp[i]))){
                   bracket++;
                }

                strcat(newCode[newCode_line] , temp[i]);

                if(isThereCloseBracket(temp[i],strlen(temp[i]))){
                    bracket--;
                }
                if(bracket != 1) newCode_line++;
                else{
                    for(int i = 0; i < loop_times; i++){
                        for(int k = 0; k < newCode_line; k++){
                            parse(newCode[newCode_line]);
                        }
                    }
                    break;
                }
            }

            /*while(fgets(line , sizeof(line) , fp)){
                tokens = split(line ," \n\r", &size);
                if(isThereOpenBracket(line,strlen(line))){
                   bracket++;
                }

                strcat(newCode[newCode_line] , line);

                if(isThereCloseBracket(line,strlen(line))){
                    bracket--;
                }
                if(bracket != 1) newCode_line++;
                else{
                    for(int i = 0; i < loop_times; i++){
                        for(int k = 0; k < newCode_line; k++){
                            parse(newCode[newCode_line]);
                        }
                    }
                    break;
                }

            }*/
        }

    }
    else{
        fprintf(stderr, "%s", "There is no keyword.\n");
        exit(-1);
    }

}

char* remove_literal(char * line , int index){
char * arr = (char*)malloc(sizeof(line) * strlen(line));
int target_index = 0;

for(int i = 0; i < strlen(line); i++){
    if(i != index) arr[target_index++] = line[i];
}
arr[target_index] = '\0';
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
		|| !strcmp(str , "move") || !strcmp(str , "from") )
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
		else if(!(isalnum(str[i])))
			return 0;
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


