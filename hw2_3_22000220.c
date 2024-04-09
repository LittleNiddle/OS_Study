#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <unistd.h>
#include <time.h>

//21901013 김예은
//프로그램을 실질적으로 실행 후 output_file의 mtime이 input_file과 같지 않고 다릅니다.
//아마 output_file을 이후에 만들고 utime을 해서 그런것 같습니다.


int main(int argc, char *argv[]) {
    char *input_file = argv[1];
    char *output_file = argv[2];

    struct stat s;
    struct utimbuf ntime;

    stat(input_file, &s);
    chmod(output_file, s.st_mode);
	chown(output_file, s.st_uid, s.st_gid);

    ntime.actime = s.st_atime;
    ntime.modtime = s.st_mtime;
    utime(output_file, &ntime);

    FILE* output_fp = fopen(output_file, "w");
    if(output_fp == NULL){
		printf("Failed to open the file %s on line %d of file %s",argv[2],__LINE__,__FILE__);
		exit(-1);
	}

    struct tm *t = localtime(&s.st_mtime);
    fprintf(output_fp,"attributes of file \"%s\"\n", input_file);
    fprintf(output_fp,"\tst_dev = %ld\n", (long)s.st_dev);
    fprintf(output_fp,"\tst_mode = %o\n", s.st_mode);
    fprintf(output_fp,"\tst_uid = %d\n", s.st_uid);
    fprintf(output_fp,"\tst_gid = %d\n", s.st_gid);
    fprintf(output_fp,"\tst_size = %lld\n", (long long)s.st_size);
    fprintf(output_fp,"\tst_mtime = %ld\n", (long)s.st_mtime);
    fprintf(output_fp,"\tmodified time = %d/%d/%d %02d:%02d:%02d\n",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);

    fprintf(output_fp,"File attributes were successfully copied.\n");

    fclose(output_fp);

    return 0;
}
