#ifndef LIB1_H

typedef struct{
    char name[50];
    long int value;
}variable;

char** split( char *line , int*);

int isKeyword(char *str);

int isAppropriateVariableName(char *str);

int isVariable(char *str);



#endif
