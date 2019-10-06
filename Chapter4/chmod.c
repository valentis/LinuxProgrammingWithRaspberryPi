#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    struct stat statbuf;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        return -1;
    }

    /* 파일에 존재 여부에 대한 정보를 가져온다. */
    if (stat(argv[1], &statbuf) < 0) {
        perror("stat");
        return -1;
    }

    /* set-group-ID를 설정하고 그룹의 실행 권한을 해제한다. */
    if (chmod(argv[1], (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
        perror("chmod");
        return -1;
    }

    /* 파일의 권한을 644("rw-r--r--") 로 설정한다. */
    if (chmod(argv[2], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
        perror("chmod");
        return -1;
    }

    return 0;
}
