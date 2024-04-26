#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	
	//DEBUG
	if(argc != 2){
		printf("Execution: %s <shared memory file>", argv[0]);
		return -1;
	}

	/*size of shared memory*/
	const int SIZE = 512;
	/*name of the shared memory object*/
	const char *name = argv[1];
	/*shared memory file descriptor*/
	int shm_fd;
	/*pointer to shared memory object*/
	char *shm;
	
	/*create the shared memory object*/
	shm_fd = shm_open(name, O_RDWR, 0666);

	/*memory map the shared memory object*/
	shm = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	char cmd[SIZE];
		
	while(1){
		printf("$ ");
		fgets(cmd, SIZE, stdin);
		cmd[strlen(cmd)-1] = '\0';
		if(cmd[0] != '\0'){
			while(shm[0]!=0) {
				usleep(100000);
			}
			if(strcmp(cmd, "exit")==0)break;
			sprintf(shm, "%s", cmd);
			if(strcmp(cmd, "exit_svr")==0) break;
		}
	}
	/*remove the shared memory object*/
	// shm_unlink(name);
	printf("Bye\n");
	return 0;
}
