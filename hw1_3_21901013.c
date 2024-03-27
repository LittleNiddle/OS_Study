#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

struct Sentence{
	char word[MAX_LEN];
	int start;
	int end;
};

int main(){
	char text[MAX_LEN] = "";
	printf("Input a text line: ");
	fgets(text, MAX_LEN, stdin);
	text[strlen(text)-1] = '\0';

	struct Sentence sentences[MAX_LEN];
	int length = strlen(text)+1;
	int count = 0;
	int i = 0;
	int start_index = 0;

	printf("                   ");

	while(text[i] != 0){
		if(isspace(text[i+1]) == 0){
			printf("[");
			i++;
			while(isspace(text[i]) == 0 && text[i] != 0){
				printf(" ");
				i++;
			}
			printf("]");
			if(text[i] != '\0') i++;
		}
		else{
			printf("[]");
			i=i+2;
		}
	}
	printf("\n\n");


	for(int j=0;j<length;j++){
		if(!isspace(text[j]) && (j == 0 || isspace(text[j-1]))){
			start_index= j;
		}
		if((isspace(text[j]) || j ==length-1) && j != start_index){
			sentences[count].start = start_index;
			sentences[count].end = j;

			int word_len = sentences[count].end - sentences[count].start;
			strncpy(sentences[count].word, &text[sentences[count].start], word_len);
			sentences[count].word[word_len] = '\0';

			count++;

			start_index = j+1;
		}
	}

	for(int k=0;k<count;k++){
		printf("words[%d] = (%d, %d, %s)\n", k, sentences[k].start, sentences[k].end, sentences[k].word);
	}
	return 0;
}
