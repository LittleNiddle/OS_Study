#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#include "Console.h"


#define DEBUG	// comment-out this line after completion

#define TRUE 1
#define FALSE 0

#define ESC 27


#define FIGHTER_LEN 3

// 22000220 남종운
// 문제가 없어 보입니다

//21901013 김예은
//Line 225의 usleep()을 10000으로 바꾸면 0.01초가 됩니다.
//또한 Line 10을 문제의 요구에 따라 주석처리하면 좋을 것 같습니다.

char fighter[] = "=B>";

typedef struct {
	int width, height;
	int key;
} FighterInfo;

int cont = TRUE;

void* MoveFighter(void *vparam);

void Erase(int x, int y, int len);
void DrawObject(int x, int y, char *object);


#define MISSLE_LEN 3
char missile[] = "-->";

typedef struct {
	int width;		// screen width
	int sx, sy;		// start coordinate
} MissileInfo;

void Shoot(int sx, int sy, int width);

int main()
{
	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	clrscr();
	gotoxy(1, screen_height + 1);
	printf("screen size = %d x %d. ", screen_width, screen_height);
	printf("press ESC to quit.");

#ifndef	DEBUG
	EnableCursor(FALSE);
#endif	//	DEBUG

	// create fighter thread
	FighterInfo fighterInfo = {screen_width, screen_height, 0 };
	pthread_t ftid = 0;

	pthread_create(&ftid, NULL, MoveFighter, &fighterInfo);

	while(fighterInfo.key != ESC){
		gotoxy(1, screen_height + 1);
		fighterInfo.key = getch();
	}

	cont = FALSE;

	gotoxy(1, screen_height);
	printf("Terminating fighter thread...");
	fflush(stdout);

	pthread_join(ftid, NULL);
	gotoxy(1, screen_height + 2);
	printf("Bye!\n");

#ifndef	DEBUG
	EnableCursor(TRUE);
#endif	//	DEBUG

	return 0;
}

void* MoveFighter(void *vparam)
{
	FighterInfo *param = (FighterInfo *)vparam;

	int x = param->width / 2;
	int y = param->height / 2;
	int dx = 0, dy = 0;

	int powerup = FALSE;

	while(cont){
		int prevx = x;
		int prevy = y;
		
		if(param->key != 0){
			switch(param->key){
			case 'i':
				dx = 0;
				dy = -1;
				break;

			case 'j':
				dx = -1;
				dy = 0;
				break;

			case 'k':
				dx = 0;
				dy = 1;
				break;

			case 'l':
				dx = 1;
				dy = 0;
				break;

			case 'p':
				powerup = 1 - powerup;
				break;

			case ' ':
				Shoot(x + FIGHTER_LEN, y, param->width);

				if(powerup){
					if(y > 1)
						Shoot(x + FIGHTER_LEN, y-1, param->width);
					if(y < param->height)
						Shoot(x + FIGHTER_LEN, y+1, param->width);
				}
				break;

			default:
				dx = dy = 0;
				break;
			}

			param->key = 0;
		}

		if(x + dx < 1 || x + dx + FIGHTER_LEN - 1 > param->width)
			dx = 0;
		if(y + dy < 1 || y + dy > param->height)
			dy = 0;

		x += dx;
		y += dy;

		Erase(prevx, prevy, FIGHTER_LEN);
		DrawObject(x, y, fighter);
		fflush(stdout);

		usleep(50000);
	}
}

void Erase(int x, int y, int len)
{
	LockDisplay();

	gotoxy(x, y);
	for(int i = 0; i < len; i++)
		putchar(' ');

	UnlockDisplay();
}

void DrawObject(int x, int y, char *object)
{
	LockDisplay();

	gotoxy(x, y);
	printf("%s", object);

	UnlockDisplay();
}

void* ThreadFn(void *vparam);

void Shoot(int sx, int sy, int width)
{
	// implement this function
	MissileInfo *param = (MissileInfo*)malloc(sizeof(MissileInfo));
	if(param == NULL){
		perror("Failed to malloc param\n");
	}

	param->sx = sx;
	param->sy = sy;
	param->width = width;

	pthread_t tid = 0;
	if(pthread_create(&tid, NULL, ThreadFn, param) != 0){
		perror("Failed to pthread_create\n");
	}
	pthread_detach(tid);
}

void* ThreadFn(void *vparam)
{
	MissileInfo *param = (MissileInfo*)vparam;

	int x = param->sx;
	int y = param->sy;
	int width = param->width;
	while(cont){
		int prevx = x;
		int prevy = y;
		x++;
		Erase(prevx, y, MISSLE_LEN);
		if(x + MISSLE_LEN -1 > width) break;
		DrawObject(x, y, missile);
		fflush(stdout);
		usleep(100000);
	}
	free(param);
	return NULL;
}
