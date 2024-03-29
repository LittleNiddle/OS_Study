#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

struct Sentence{
	char word[MAX_LEN];
	int start;
	int end;
};

// 21900810 황건하
// 여러개의 space가 들어갈 경우 []가 출력되며 space가 word로 출력됨
// 이전의 char와 현재 char가 whitespace인지 character인지 비교하는 알고리즘을 이용하면 될것이다.

// 22000220 남종운(늦게 보내드려 죄송합니다..)
// 처음에 여러 개의 whitespace가 입력되었을 때, 이를 문자로 인식합니다.
// 처음 입력을 받을 때, whitespace인지 확인하는 과정이 필요할 것 같습니다.

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
