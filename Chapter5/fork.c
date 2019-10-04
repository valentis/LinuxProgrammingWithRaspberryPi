#include <stdio.h>
#include <unistd.h>

static int g_var = 1; 	/* data 영역의 초기화된 변수 */
char str[ ] = "PID";

int main(int argc, char** argv)
{
    int var; 		/* stack 영역의 지역 변수 */
    pid_t pid;

    var = 92;

    if((pid = fork()) < 0) { 	/* fork( ) 함수의 에러 시 처리 */
        perror("[ERROR] : fork()");
    } else if(pid == 0) { 	/* 자식 프로세스에 대한 처리 */
        g_var++; 		/* 변수의 값 변경 */
        var++;
        printf("Parent %s from Child Process(%d) : %d\n",
                                     str, getpid(), getppid());
    } else { 			/* 부모 프로세스에 대한 처리 */
        printf("Child %s from Parent Process(%d) : %d\n", str, getpid(), pid);
        sleep(1);
    }

    /* 변수의 내용을 출력 */
    printf("pid = %d, Global var = %d, var = %d\n", getpid(), g_var, var);

    return 0;
}
