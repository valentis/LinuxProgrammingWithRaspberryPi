#include <stdio.h>
#include <time.h>
#include <sys/time.h> 			/* getimeofday( ) 함수에서 사용 */
#include <stdlib.h>

int main(int argc, char **argv)
{
    int i, j;
    time_t rawtime;
    struct tm *tm;
    char buf[BUFSIZ];
    struct timeval mytime;

    time(&rawtime); 			/* 현재의 시간 구하기 */
    printf("time : %u\n", (unsigned)rawtime); 	/* 현재의 시간을 화면에 출력 */

    gettimeofday(&mytime, NULL); 	/* 현재의 시간 구하기 */
    printf("gettimeofday : %ld/%d\n", mytime.tv_sec, mytime.tv_usec);
    printf("ctime : %s", ctime(&rawtime)); 	/* 현재의 시간을 문자열로 바꿔서 출력 */
    
    putenv("TZ=PST3PDT"); 		/* 환경 변수를 설정한다. */
    tzset(); 				/* TZ 변수를 설정한다. */
    tm = localtime(&rawtime); 		/* tm = gmtime(&rawtime); */
    printf("asctime : %s", asctime(tm)); 	/* 현재의 시간을 tm 구조체를 이용해서 출력 */

    strftime(buf, sizeof(buf), "%a %b %e %H:%M:%S %Y", tm); 	/* 사용자 정의 문자열 지정 */
    printf("strftime : %s\n", buf);

    return 0;
}
