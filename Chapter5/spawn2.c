#include <stdio.h>
#include <sys/wait.h>
#include <spawn.h>

extern char **environ;

int system(char *cmd) 		/* posix_spawn() 함수를 사용 */
{
    pid_t pid;
    int status;
    posix_spawn_file_actions_t actions;
    posix_spawnattr_t attrs;
    char *argv[] = {"sh", "-c", cmd, NULL};

    posix_spawn_file_actions_init(&actions);
    posix_spawnattr_init(&attrs);
    posix_spawnattr_setflags(&attrs, POSIX_SPAWN_SETSCHEDULER);
    posix_spawn(&pid, "/bin/sh", &actions, &attrs, argv, environ);

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
