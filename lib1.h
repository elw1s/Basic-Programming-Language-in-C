#ifndef LIB1_H

typedef struct{
    char name[50];
    long int value;
}variable;

char** split( char *line , int*);

void parse(char* file_name);

int isSpecialKeyword(char *str);

int isAppropriateVariableName(char *str);

int isVariable(char *str);

int is_existing_variable(char *str);

int isInt(char * str , int size);

char* remove_literal(char * str);

int isThereOpenBracket(char * line , int str_len);

int isThereCloseBracket(char * line , int str_len);

long int getValue(char *str);


#endif
