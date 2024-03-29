#include <stdio.h>
#include <string.h>

#define MAX_LEN 256

// 21900810 황건하
// Struct 사용 안함
// white space 고려 안함
// pre_char 부분을 isspace를 활용한 int variable로 만들면 whitespace를 해결할 수 있을 것이다.

int main()
{
	// read a text from the user
	char text [MAX_LEN]="";
	printf("input a text: ");
	fgets(text,MAX_LEN,stdin);
	text[strlen(text)-1] = 0;

	// fill space with blanks
	printf("              ");

	// display mark to check start and end
	char pre_char = ' ';
	char words[50][128] = {"",};
	int start_idx[50] = {0,};
	int row = 0, col = 0;
	for(int i=0; i<strlen(text); i++){
		if(pre_char == ' ') {
			if(text[i] != ' '){
				printf("[");	
				words[row][col] = text[i];
				start_idx[row] = i;
				col++;
			}
			else printf(" ");
		}	
		else if(text[i] == ' '){
			printf("]");
			row++;
			col = 0;
		}
		else {
			printf(" ");
			words[row][col] = text[i];
			col++;
		}
		pre_char = text[i];
	}
	if(pre_char != ' '){
		printf("]");
		row++;
	}
	printf("\n\n");


	for(int i=0; i<row; i++){
		printf("words[%d] = (%d, %d, %s)\n",i,start_idx[i],start_idx[i]+(int)strlen(words[i]),words[i]);
	}

	return 0;
}
