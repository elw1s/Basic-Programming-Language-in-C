#ifndef LIB1_H

typedef struct{
    char name[50];
    long int value;
}variable;

char** split( char *line ,char * delimiter, int*);

void parse(char* line);

int isSpecialKeyword(char *str);

int isAppropriateVariableName(char *str);

int isVariable(char *str);

int is_existing_variable(char *str);

int isInt(char * str , int size);

char* remove_literal(char * str , int index);

int isThereOpenBracket(char * line , int str_len);

int isThereCloseBracket(char * line , int str_len);

long int getValue(char *str);

void setValue(char * str , int value);

void read(char *file_name);

void removeCommentLines(char* code);

int substring(char *source, int from, int n, char *target);

#endif
