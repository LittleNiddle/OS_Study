#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>

#define BUF_LEN 256

// 21901013 김예은
// 문제를 찾지 못했습니다.

// 22000220 남종운
// 수정할 부분이 없어 보입니다.

void execute_cmd(char* cmd);

int main(int argc, char *argv[]){
	
	//DEBUG
	if(argc != 2){
		printf("Execution: %s <shared memory file>", argv[0]);
		return -1;
	}
	/*name of the shared memory object*/
	const char *name = argv[1];
	/*size of shared memory*/
	const int SIZE = 512;
	//*shared memory file descriptor*/
	int shm_fd;
	/*pointer to shared memory object*/
	char *shm;
	/*string written to shared memory*/
	char cmd[BUF_LEN];	
	
	/*create the shared memory object*/
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/*configgure the size of the shared memory object*/
	ftruncate(shm_fd, SIZE);

	/*memory map the shared memory object*/
	shm = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	/*initialize the shared memory object*/
	sprintf(shm, "%s", cmd);
	//printf("shm : %s\n", shm);
	shm[0] = 0;
	
	printf("Welcome to my remote shell server!\n");
	/*repeat*/
	while(1){
		printf("Waiting for a command...\n");
		while(shm[0]==0){
			usleep(100000);
		}
		strcpy(cmd, shm);
		if(strcmp(cmd, "exit_svr") == 0) break;
		//printf("%s\n", cmd);
		execute_cmd(cmd);
		shm[0] = 0;
	}
	shm_unlink(name);
	printf("Bye!\n");
	return 0;
}

void execute_cmd(char *clt_cmd){
	char *cmd = clt_cmd;
    char *argv[BUF_LEN];
    int argc = 0;
    int start_idx[BUF_LEN];
    int str_len = strlen(cmd);
	char pre_char = ' ';
	
	for(int i=0; i<str_len; i++){
		if(isspace(pre_char)||pre_char=='\0'){
			if(!isspace(cmd[i])){
				argv[argc] = &cmd[i];
				start_idx[argc] = i;
				argc++;
			}
		}
		else if(isspace(cmd[i])){
			cmd[i] = '\0';
		}
		else{
		// printf("word");
		}
		pre_char = cmd[i];
	}
	argv[argc] = NULL;
	
	if(argv[0] != NULL){
		if(strcmp(argv[0], "cd")==0){
			int cd_result = chdir(argv[1]);
			if(cd_result != 0) printf("cd error\n");
		}
		else{
			pid_t child_pid = fork();
			if(child_pid < 0){
				fprintf(stderr, "fork failed\n");
				exit(-1);
			}
			else if(child_pid == 0){
				execvp(argv[0], argv);
				exit(0);
			}
			else{
				wait(NULL);
			}
		}
	}
}

