#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256
#define WORD_LEN 32
#define WORD_COUNT 256

typedef struct {
	int start;
	int end;
	char word[WORD_LEN];
}WORD;

int main(){
	char text[MAX_LEN];
	WORD words[MAX_LEN];

        // state 0: word start - write [
        // state 1: word - write " "
        // state 2: word end - write ]
        // state 3: blank world - write " "
        int state = 0;
        int count = 0;
        int wordCount = -1;
        int checkWS = 0;

        int start = 0;

        printf("input a text: ");
        fgets(text, MAX_LEN, stdin);
        text[strlen(text) - 1] = 0;

        printf("              ");
        
        for(;count<strlen(text);count++){
            checkWS = isspace(text[count]);
            // white space
            if(checkWS != 0){
                if(state == 0 || state == 1) state = 2;
                else if(state == 2 || state == 3) state = 3;
                else printf("error in blank");
            }
            // character
            else{
                if(state == 0 || state == 1) {
                    if(count == 0) state = 0; // 처음
                    else state = 1;
                }
                else if(state == 3 || state == 2) state = 0;
                else printf("error in not blank");
            }

            //write
            if(state == 0){
                printf("[");
                wordCount++;

                words[wordCount].start = count;
                start = words[wordCount].start;
                words[wordCount].word[count-start] = text[count];
            }
            else if(state == 1){
                printf(" ");

                start = words[wordCount].start;
                words[wordCount].word[count-start] = text[count];
            }
            else if(state == 2){
                printf("]");

                start = words[wordCount].start;
                words[wordCount].end = count;
                words[wordCount].word[count-start] = '\0';
            }
            else{
                printf(" ");
            }
        }

        // last word
        printf("]\n");
        words[wordCount].end = count;
        words[wordCount].word[count-start] = '\0';

        for(int i=0; i<=wordCount; i++){
            printf("words[%d] = (%d, %d, %s)\n", i, words[i].start, words[i].end, words[i].word);
        }

        return 0;
}
