#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>


#define MAX_LEN 50
#define SIZE 512

// 22000220 남종운
// 수정할 부분이 없어 보입니다.

int main(int argc, char *argv[]){
    char *shm_filename = argv[1]; //shared memeory file name
	void *addr; //shared memory address
	char command_line[256];

    
    //create shared memory object
    int fd = shm_open(shm_filename, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
        perror("Failed shm_open.\n");
        exit(EXIT_FAILURE);
    }


    //resize the shared memory object
    if(ftruncate(fd, SIZE) == -1){
        perror("Ftruncate error.\n");
        exit(EXIT_FAILURE);
    }


    //map the shared memeory object
    addr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED){
        perror("Mmap error.\n");
        exit(EXIT_FAILURE);
    }


    //Initialize shared memory
    char *shm = (char *)addr;
    shm[0] = 0;


    printf("Welcome to my remote shell server!\n");
    while(1){
        printf("Waiting for a command... \n");

        while(shm[0] == 0){
            usleep(100000);
		}

		strcpy(command_line,shm);

		command_line[strcspn(command_line,"\n")] = '\0';

        //Change whitespace into NULL
        for(int i=0; command_line[i] != '\0'; i++) {
            if(isspace(command_line[i])){
             command_line[i] = '\0';
            }
        }

        //Cut string NULL being the standard
        char *token = command_line;
        int argc = 0;
        while(*token != '\0' && argc < MAX_LEN -1){
            argv[argc++] = token;
            token += strlen(token)+1;
        }

        //last NULL
        argv[argc] = NULL;
        

        //Reset shared memeory
        shm[0] = 0;

        //Exit case
        if(strcmp(argv[0],"exit_svr") == 0){
			printf("Bye!\n");
			break;
		}
		

        //cd case
        if(strcmp(argv[0],"cd") == 0){
            if(chdir(argv[1])){
                perror("Failed to change directory.\n");
            }
            continue;
        }

        //not empty
        else if(argv[0] != NULL){
            //Child process create
            pid_t pid = fork();

            if(pid<0){
                perror("Failed to create child process.\n");
                exit(1);
            } else if(pid == 0){
                if(execvp(argv[0],argv)){
                    perror("Failed execvp.\n");
                }
                if(strcmp(argv[0],"exit") == 0){
                    exit(0);
                }
            }
            else{
                //Parent process
                wait(NULL);
            }
        }
    }

    if(shm_unlink(shm_filename) == -1){
        perror("Failed to remove shared file object.\n");
    }

    return 0;
}
