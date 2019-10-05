#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>

int main(int argc, char** argv)
{
    struct sigaction sa; /* 시그널 처리를 위한 시그널 액션 */
    struct rlimit rl;
    int fd0, fd1, fd2, i;
    pid_t pid;

    if(argc < 2) {
        printf("Usage : %s command\n", argv[0]);
        return -1;
    }

    /* 파일 생성을 위한 마스크를 0으로 설정 */
    umask(0);

    /* 사용할 수 있는 최대의 파일 디스크립터 수 얻기 */
    if(getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        perror("getlimit()");
    }

    if((pid = fork()) < 0) {
        perror("error()");
    } else if(pid != 0) { /* 부모 프로세스는 종료한다. */
        return 0;
    }

    /* 터미널을 제어할 수 없도록 세션의 리더가 된다. */
    setsid();

    /* 터미널 제어와 관련된 시그널을 무시한다. */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGHUP, &sa, NULL) < 0) {
        perror("sigaction() : Can't ignore SIGHUP");
    }

    /* 프로세스의 워킹 디렉터리를 ‘/’로 설정한다. */
    if(chdir("/") < 0) {
        perror("cd()");
    }

    /* 프로세스의 모든 파일 디스크립터를 닫는다. */
    if(rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }

    for(i = 0; i < rl.rlim_max; i++) {
        close(i);
    }

    /* 파일 디스크립터 0, 1과 2를 /dev/null로 연결한다. */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /* 로그 출력을 위한 파일 로그를 연다. */
    openlog(argv[1], LOG_CONS, LOG_DAEMON);
    if(fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        return -1;
    }

    /* 로그 파일에 정보 수준의 로그를 출력한다. */
    syslog(LOG_INFO, "Daemon Process");
    while(1) {
        /* 데몬 프로세스로 해야 할 일을 반복 수행 */
    }

    /* 시스템 로그를 닫는다. */
    closelog();

    return 0;
}
