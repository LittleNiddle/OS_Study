#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <unistd.h>
#include <pthread.h>

#include "Console.h"

#define DEBUG		// comment-out this line after completion

#define TRUE 1
#define FALSE 0

typedef struct {
	int left, top, right, bottom;
} MyRect;

typedef struct {
	int x;
	int height;

	MyRect *rect;
} ThreadParam;

//21901013 김예은
//12번째 줄을 주석처리하면 좋을것 같습니다.
//그외 수정할 부분을 찾지 못했습니다.

void *MoveVertical(void *vparam);
int PointInRect(MyRect *rect, int x, int y);
char GetRandomChar();

int cont = TRUE;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	srand(time(NULL));
	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	MyRect critical_region = { screen_width / 4, screen_height * 3 / 7, 
								screen_width * 3 / 4, screen_height * 4 / 7 };

	clrscr();

	DrawLine(critical_region.left, critical_region.top, critical_region.right, critical_region.top, '*');
	DrawLine(critical_region.left, critical_region.bottom, critical_region.right, critical_region.bottom, '*');

	gotoxy(1, screen_height + 1);
	printf("Press ESC to quit!");

#ifndef	DEBUG
	EnableCursor(FALSE);
#endif	//	DEBUG

	pthread_t *tid = (pthread_t *)malloc(screen_width * sizeof(pthread_t));
	ThreadParam *param = (ThreadParam *)malloc(screen_width * sizeof(ThreadParam));
	if(tid == NULL || param == NULL){
		printf("Failed to allocate memory in line %d\n", __LINE__);
		exit(-1);
	}

	for(int t = 0; t < screen_width; t++){
		param[t].x = t + 1;
		param[t].height = screen_height;
		param[t].rect = &critical_region;

		pthread_create(&tid[t], NULL, MoveVertical, &param[t]);
	}

	while(!kbhit() || getch() != 27)
		usleep(10000);

	cont = FALSE;

	for(int t = 0; t < screen_width; t++)
		pthread_join(tid[t], NULL);

	int status = pthread_mutex_destroy(&mutex);
	free(tid);
	free(param);

	gotoxy(1, screen_height);
	printf("Bye!\n");

#ifndef	DEBUG
	EnableCursor(TRUE);
#endif	//	DEBUG

	return 0;
}

void *MoveVertical(void *vparam)
{
	ThreadParam *param = (ThreadParam *)vparam;
	int x = param->x;
	int height = param->height;

	while(cont){
		int heady = 1;
		int len = rand() % 15 + 5;
		int delay = (rand() % 30 + 10) * 10000;
		while(cont && heady - len + 1 <= height){
			if(heady <= height){
				PrintXY(x, heady, "%c", GetRandomChar());
			}
			if(PointInRect(param->rect, x, heady)==FALSE && PointInRect(param->rect, x, heady+1)==TRUE)
				pthread_mutex_lock(&mutex);

			

			int taily = heady - len + 1;
			if(taily >= 1){
				if(x >= param->rect->left && x <= param->rect->right
					&& (taily == param->rect->top || taily == param->rect->bottom))
					PrintXY(x, taily, "*");
				else
					PrintXY(x, taily, " ");
			}
			if(PointInRect(param->rect, x, taily)==TRUE && PointInRect(param->rect, x, taily+1)==FALSE)
				pthread_mutex_unlock(&mutex);

			fflush(stdout);

			usleep(delay);

			heady++;
		}
	}

	return NULL;
}

int PointInRect(MyRect *rect, int x, int y)
{
	if(x >= rect->left && y >= rect->top && x <= rect->right && y <= rect->bottom)
		return TRUE;

	return FALSE;
}

char GetRandomChar()
{
	char c = 0;
	do
		c = rand() % 128;
	while(!isprint(c));

	return c;
}

