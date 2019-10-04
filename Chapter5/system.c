#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int system(const char *cmd) 	/* fork(), exec(), waitpid() 함수를 사용 */
{
    pid_t pid;
    int status;

    if((pid = fork()) < 0) { 	/* fork( ) 함수 수행 시 에러가 발생했을 때의 처리 */
        status = -1;
    } else if(pid == 0) { 	/* 자식 프로세스의 처리 */
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127); 		/* execl( ) 함수의 에러 사항 */
    } else { /* 부모 프로세스의 처리 */
        while(waitpid(pid, &status, 0) < 0) 	/* 자식 프로세스의 종료 대기 */
        if(errno != EINTR) { 	/* waitpid( ) 함수에서 EINTR이 아닌 경우의 처리 */
            status = -1;
            break;
        }
    }

    return status;
}

int main(int argc, char** argv, char **envp)
{
    while(*envp) 		/* 환경 변수를 출력한다. */
        printf("%s\n", *envp++);

    system("who"); 		/* who 유틸리티 수행 */
    system("nocommand"); 	/* 오류사항의 수행 */
    system("cal"); 		/* cal 유틸리티 수행 */

    return 0;
}
