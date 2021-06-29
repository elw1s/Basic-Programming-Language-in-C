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



#endif
