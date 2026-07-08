#include <stdio.h>
#include <string.h>
#include "paraser.h"
int wordsepstrer(const char *inputstrin,char outputstring[100][100]);
int textchoper(const char *inputstring, int screenw, char ouputstring[100][100]);


int wordsepstrer(const char *inputstring, char ouputstring[100][100]){
	    int j,i,k = 0;
	    for(j = 0;inputstring[k] != '\0';j++){
		for(i = 0; inputstring[k] != ' ';i++){
			ouputstring[j][i] = inputstring[k];
			k++;
			if(inputstring[k] == '\0') break;
		}
	    if (inputstring[k] == ' '){
		ouputstring[j][i] = '\0';
		k++;
	}
	    }
        ouputstring[j - 1][i + 1]='\0';
    return j;
}


int textchoper(const char *inputstring, int screenw, char ouputstring[100][100]){
	int j,i,k = 0;	
	char wordsepstr[100][100];
	int numofword = wordsepstrer(inputstring,wordsepstr);
	int linenum;
	if((strlen(inputstring) - numofword) % screenw == 0){linenum = (strlen(inputstring) - numofword) / screenw;}else{linenum = ((strlen(inputstring) - numofword) / screenw) +1;}
	k = 0;
	int h = 0;
	int writtensize = 0;
	i = 0;
		for(int j = 0;j < numofword;j++){
			writtensize += strlen(wordsepstr[j]);
			for(int h = 0;h < strlen(wordsepstr[j]);h++){
				ouputstring[i][k] = wordsepstr[j][h];
				k++;
			}
			ouputstring[i][k]= ' ';
			k++;
			if(writtensize >= screenw){
				ouputstring[i][k] = '\0';
				k = 0;
				writtensize = 0;
				i++;
			}

		}
		ouputstring[i][k] = '\0';
	return linenum;
}




