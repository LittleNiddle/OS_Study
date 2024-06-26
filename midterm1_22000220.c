#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>


#include "Console.h"


//#define DEBUG			// comment out this line after completion

//21901013 김예은
//문제의 요구사항에 의해 Line 88에 작성한 usleep을 Line 50에 else 내에 작성해야 될 것 같습니다.

// 21900810 황건하
// 이상 없는것 같습니다.

//(wrtie your code here and do not modify other parts)
#define TRUE 1
#define FALSE 0

void DropWord(char *word, int idx, int no_word, int width, int height);
void Erase(int sx, int sy, int len);

int main(int argc, char *argv[])
{
	if(argc == 1){
		printf("Usage: %s <word1> <word2> ...\n", argv[0]);
		return 0;
	}

	clrscr();

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	gotoxy(1, screen_height + 1);
	printf("screen size = %dx%d.\n", screen_width, screen_height);

#ifndef	DEBUG
	EnableCursor(FALSE);
#endif	//	DEBUG

	// write your code here and do not modify other parts
	for(int i=1; i<argc; i++){
		pid_t pid = fork();
		if(pid > 0)
			usleep(1000000);
		else if(!pid){
			DropWord(argv[i],i,argc-1,screen_width,screen_height);
			exit(0);
		}
		else{
			perror("fork");
			exit(-1);
		}
	}
	


	gotoxy(1, screen_height + 2);
	printf("Bye!\n");

#ifndef	DEBUG
	EnableCursor(TRUE);
#endif	//	DEBUG

	return 0;
}

void DropWord(char *word, int idx, int no_word, int width, int height)
// drops a word from top to bottom.
// read the code to understand the parameters
{
	int len = strlen(word);
	int x = width * idx / (no_word + 1);
	for(int y = 1; y < height; y++){

		LockDisplay();

		if(y > 1)
			Erase(x, y - 1, len);

		gotoxy(x, y);
		printf("%s", word);

		fflush(stdout);

		UnlockDisplay();

		usleep(100000);
	}
}

void Erase(int sx, int sy, int len)
{
	gotoxy(sx, sy);
	for(int i = 0; i < len; i++)
		putchar(' ');
}

