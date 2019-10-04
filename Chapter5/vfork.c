#include <stdio.h>
#include <unistd.h>

static int g_var = 1; 	/* 초기화된 data 영역의 전역 변수 */
char str[ ] = "PID";

int main(int argc, char** argv)
{
    int var; 		/* 스택(stack) 영역의 자동 변수 */

    pid_t pid;
    var = 88;

    if((pid = vfork()) < 0) { 	/* fork() 함수 호출 에러 */
        perror("vfork()");
    } else if(pid == 0) { 	/* 자식 프로세스 */
        g_var++; 		/* 변수의 값 변경 */
        var++;
        printf("Parent %s from Child Process(%d) : %d\n",
        str, getpid(), getppid());
        printf("pid = %d, Global var = %d, var = %d\n", getpid(), g_var, var);
        _exit(0);
    } else { 			/* 부모 프로세스 */
        printf("Child %s from Parent Process(%d) : %d\n", str, getpid(), pid);
    }

    printf("pid = %d, Global var = %d, var = %d\n", getpid(), g_var, var);

    return 0;
}
