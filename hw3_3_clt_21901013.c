#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>


#define SIZE 512
#define LEN 100

// 22000220 남종운
// 수정할 부분이 없어 보입니다.

int main(int argc, char *argv[]){
    char command_line[LEN];
    char *shm_filename = argv[1]; //shared memeory file name
    void *addr; //shared memory address

    
	printf("Welcome to my remote shell client!\n");
    //open shared memory object
    int fd = shm_open(shm_filename, O_RDWR, 0666);
    if(fd == -1){
        perror("Failed shm_open.\n");
        exit(EXIT_FAILURE);
    }


    //map the shared memeory object
    addr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED){
        perror("Mmap error.\n");
        exit(EXIT_FAILURE);
    }

    //Initialize shared memory
    char *shm = (char *)addr;
    shm[0] = 0;

	printf("Connected to server via shared memory.\n");
    while(1){
		printf("$ ");
        fgets(command_line, LEN, stdin);


		if(strlen(command_line) != 0){
			while(shm[0] != 0) usleep(10000);

            if(strcmp(&command_line[0],"exit\n") == 0){
				printf("Bye!\n");
				break;
			}

			strcpy(shm, command_line);
            if(strcmp(&command_line[0],"exit_svr\n") == 0){
				break;
			}
        }
	}
	if(munmap(shm, SIZE) == -1){
        perror("Failed remove shared file object.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
