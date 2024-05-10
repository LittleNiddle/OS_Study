#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "Console.h"

#define DEBUG					// comment-out after completion

#define TRUE 1
#define FALSE 0

#define MAX(x, y)	((x) >= (y) ? (x) : (y))
#define MIN(x, y)	((x) <= (y) ? (x) : (y))
#define ABS(x) 		((x) >= 0 ? (x) : (-(x)))

#define MAX_CHAR 512

typedef struct {
	int sx, sy;		// starting coordinate
	int ex, ey;		// destination coordinate
	int x, y;		// current coordinate
	char c;
} CharInfo;

void Animation(char *mesg, int width, int height);
void Erase(int sx, int sy, int len);


int main()
{
	srand(time(NULL));

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	clrscr();
	gotoxy(1, 1);
	printf("screen size = %d x %d.\n", screen_width, screen_height);

#ifndef	DEBUG
	EnableCursor(FALSE);
#endif	//	DEBUG
	// Add shm
	const char *shm_filename = "abcd";
	const int SIZE = 512;
	int shm_fd;
	char* mesg;
	// char *shm;
	// char cmd[MAX_CHAR];

	shm_fd = shm_open(shm_filename, O_CREAT | O_RDWR, 0666);
	
	if(ftruncate(shm_fd, SIZE) == -1){
        perror("Ftruncate error.\n");
        exit(EXIT_FAILURE);
    }
	
	mesg = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(mesg == MAP_FAILED){
        perror("Mmap error.\n");
        exit(EXIT_FAILURE);
    }
	
	mesg[0] = 0;

// DO NOT MODIFY THE FOLLOWING PART
	pid_t pid = fork();
	if(pid == 0){
		// the child process animates the message
		Animation(mesg, screen_width, screen_height);
		exit(0);
	} else if(pid < 0){
		printf("fork() failed in line %d\n", __LINE__);
		exit(-1);
	} else {
		// the parent process reads messages from the user and sends it to the child process
		char tmp_mesg[MAX_CHAR] = "";
		int i = 0;
		while(1){
			gotoxy(1, screen_height + 1);
			printf("Input text: %s ", tmp_mesg);	// the last ' ' is for backspace
			fflush(stdout);

			int c = getch();
			switch(c){
			case 127:				// backspace
				if(i > 0)
					tmp_mesg[--i] = 0;
				break;

			case '\n':				// enter
				if(i > 0){
					for(int j = i; j >= 0; j--)		// copy in reverse order to set mesg[0] to zero last
						mesg[j] = tmp_mesg[j];

					tmp_mesg[0] = 0;
					i = 0;
					Erase(1, screen_height + 1, screen_width);
				}
				break;

			default:
				tmp_mesg[i++] = c;
				tmp_mesg[i] = 0;
				break;
			}

			if(strcmp(mesg, "exit") == 0)
				break;
		}
	}

	gotoxy(1, screen_height);
	printf("parent is terminating\n");

	wait(NULL);
// DO NOT MODIFY UP TO HERE
	// Deallocate shm
	munmap(mesg, SIZE);
    close(shm_fd);
	shm_unlink(shm_filename);
	
	gotoxy(1, screen_height + 2);
	printf("Bye!\n");

#ifndef	DEBUG
	EnableCursor(TRUE);
#endif	//	DEBUG

	return 0;
}

void Animation(char *mesg, int width, int height)
// DO NOT MODIFY THIS FUNCTION.
{
	char local_mesg[MAX_CHAR] = "Input text to animate! (type \"exit\" to quit.)";

	while(1){
		if(mesg[0] != 0){
			strcpy(local_mesg, mesg);
			mesg[0] = 0;
		}

		if(strcmp(local_mesg, "exit") == 0)
			break;

		int mesg_len = strlen(local_mesg);

		CharInfo chars[MAX_CHAR] = { 0 };
		int n = 0;
		int max_dist = 0;
		for(int i = 0; local_mesg[i]; i++){
			char c = local_mesg[i];
			if(isspace(c))
				continue;
			chars[n].sx = chars[n].x = random() % (width - 1) + 2;
			chars[n].sy = chars[n].y = random() % (height - 1) + 2;
			chars[n].ex = (width - mesg_len) / 2 + i;
			chars[n].ey = height / 2;
			chars[n].c = c;

			int dist = MAX(ABS(chars[n].ex - chars[n].sx), ABS(chars[n].ey - chars[n].sy));
			max_dist = MAX(max_dist, dist);

			gotoxy(chars[n].x, chars[n].y);
			putchar(chars[n].c);

			n++;
		} 

		for(int t = 1; t <= max_dist; t++){
			usleep(30000);

			for(int i = 0; i < n; i++){
				int prevx = chars[i].x;
				int prevy = chars[i].y;

				chars[i].x = ((max_dist - t) * chars[i].sx + t * chars[i].ex + max_dist / 2) / max_dist;
				chars[i].y = ((max_dist - t) * chars[i].sy + t * chars[i].ey + max_dist / 2) / max_dist;
	
				if(chars[i].x != prevx || chars[i].y != prevy){
					gotoxy(prevx, prevy);
					putchar(' ');

					gotoxy(chars[i].x, chars[i].y);
					putchar(chars[i].c);

					fflush(stdout);
				}
			}
		}

		gotoxy((width - mesg_len) / 2, height / 2);
		printf("%s", local_mesg);
		fflush(stdout);

		if(strcmp(mesg, "exit") == 0)			// to stop immediately on "exit"
			break;

		sleep(2);

		Erase(1, height / 2, width);
		fflush(stdout);
	}

	gotoxy(1, height + 1);
	printf("child is terminating\n");
}

void Erase(int sx, int sy, int len)
// DO NOT MODIFY THIS FUNCTION.
{
	gotoxy(sx, sy);
	for(int i = 0; i < len; i++)
		putchar(' ');
}
