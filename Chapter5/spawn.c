#include <stdio.h>
#include <sys/wait.h>
#include <spawn.h>

extern char **environ;

int system(char *cmd) 		/* posix_spawn() 함수를 사용 */
{
    pid_t pid;
    int status;

    char *argv[] = {"sh", "-c", cmd, NULL};
    posix_spawn(&pid, "/bin/sh", NULL, NULL, argv, environ);
    waitpid(pid, &status, 0);

    return status;
}

int main(int argc, char** argv, char **envp)
{
    while( *envp) 		/* 환경 변수를 출력한다. */
        printf( "%s\n", *envp++);

    system("who"); 		/* who 유틸리티 수행 */
    system("nocommand"); 	/* 오류사항의 수행 */
    system("cal"); 		/* cal 유틸리티 수행 */

    return 0;
}
