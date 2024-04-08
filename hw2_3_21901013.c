#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <utime.h>
#include <time.h>

#define MAX_LEN 1024

// 22000220 남종운
// 원래 존재하는 파일에 대한 수정을 할 때 오류가 발생합니다.
// Line 36 des_file을 열 때 O_EXCL을 삭제하면 정상적으로 작동할 것 같습니다.

int main(int argc, char *argv[]) {
  const char *source_file, *des_file;
  int s_state, d_state;

  struct stat stat_buff, des_buff;   //for stat()
  
  
  source_file = argv[1];
  des_file = argv[2];

  
  s_state=open(source_file, O_RDONLY);  //sucess > 0, fail = -1

  if(s_state == -1){
    perror("Failed to open file original file.\n");
    exit(EXIT_FAILURE);
  }

  d_state=open(des_file, O_WRONLY | O_CREAT | O_EXCL, 0644);
  //0644 = owner is able to read, write while others are only able to read
 
  if(d_state == -1){
    perror("Failed to open or create the copying file.\n");
    close(d_state);
    exit(EXIT_FAILURE);
  }

  if(stat(source_file,&stat_buff) == -1){
    perror("Failed to create the metatdata file.");
    exit(EXIT_FAILURE);
  }

  if(stat(des_file,&des_buff) == -1){
    perror("Failed to create the metadat file of destination file.");
    exit(EXIT_FAILURE);
  }

  //Change time

  struct tm *timeinfo;  //for localtime
  struct utimbuf rawtime;    //contains time_t type

   rawtime.actime = stat_buff.st_atime;
   rawtime.modtime = stat_buff.st_mtime;
   timeinfo = localtime(&rawtime.modtime);

    utime(des_file, &rawtime);
    chmod(des_file,stat_buff.st_mode);
    chown(des_file,stat_buff.st_uid,stat_buff.st_gid);


    //printing
    printf("Attributes of file \"%s\"\n",source_file);
    printf("    st_dev = %ld\n",stat_buff.st_dev);
    printf("    st_mode = %o\n",stat_buff.st_mode); 
    printf("    st_uid = %u\n",stat_buff.st_uid);
    printf("    st_gid = %u\n",stat_buff.st_gid);
    printf("    st_size = %ld\n",stat_buff.st_size);
    printf("    st_mtime = %ld\n",stat_buff.st_mtime);
    printf("    modified time = %d/%02d/%02d %02d:%02d:%02d\n",
        timeinfo->tm_year + 1900,  // Year
        timeinfo->tm_mon + 1,      // Month
        timeinfo->tm_mday,         // Day
        timeinfo->tm_hour,         // Hour
        timeinfo->tm_min,          // Minute
        timeinfo->tm_sec);
	printf("File attributes were successfully copied.\n");
  
return 0;

}
