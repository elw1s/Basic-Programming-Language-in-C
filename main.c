#include <stdio.h>

void main(){

	char line[150];

	FILE *f = fopen("x.ba","r");

	if(f == NULL){
		printf("File cannot opened.\n");
		return;
	}

	while(fgets(line , 150 , f)){
		printf("%s", line);
	}
}

