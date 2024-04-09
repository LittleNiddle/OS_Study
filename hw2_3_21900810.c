#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>

#define BUF_SIZE 1024

// 22000220 남종운
// 파일의 속성 정보들이 <dest_file>에 들어가지 않고 터미널에 바로 출력됩니다.
// char* dst_file을 이용하여 파일을 열면 쉽게 수정 가능할 것 같습니다.

//21901013 김예은
//dst_file의 atime이 src_file이랑 같지 않습니다.

int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("Execution: %s <source_file> <destination_file>", argv[0]);
		return -1;
	}
	
	char *src_file = argv[1];
	char *dst_file = argv[2];
	
	struct stat st;
	struct tm *time;
	struct utimbuf ubuf;
	
	if (stat(src_file, &st) == -1)
		printf("Error in stat");
	
	time = localtime(&st.st_mtime);
	
	ubuf.actime = st.st_atime;
	ubuf.modtime = st.st_mtime;
	if(utime(dst_file, &ubuf)==-1)
		printf("Error in utime\n");
	
	int year;
	if((year = time->tm_year%100) > 1) year+=2000;
	else year+=1900;

	printf("attributes of file \"%s\"\n", src_file);
	printf("st_dev = %ld\n", (unsigned long)st.st_dev);
	printf("st_mode = %lo\n", (unsigned long)st.st_mode);
	printf("st_gid = %d\n", st.st_gid);
	printf("st_uid = %d\n", st.st_uid);
	printf("st_size = %ld\n", (long)st.st_size);
	printf("st_mtime = %ld\n", (long)st.st_mtime);
	printf("modified time = %d/%d/%d %d:%d:%d\n", year, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
	
	if(chmod(dst_file, st.st_mode) == -1)
		printf("Error in chomd\n");
	if(chown(dst_file, st.st_uid, st.st_gid) == -1)
		printf("Error in chown\n");

	printf("File attributes were successfully copied.\n");

	return 0;
}
