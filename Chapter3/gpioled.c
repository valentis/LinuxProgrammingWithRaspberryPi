#include <stdio.h>
#include <stdlib.h> 	/* atoi( ) 함수 */
#include <string.h>
#include <fcntl.h>
#include <unistd.h> 	/* close( ) 함수 */

int ledControl(int gpio)
{
    int fd;
    char buf[BUFSIZ];

    fd = open("/sys/class/gpio/export", O_WRONLY); 		/* 해당 GPIO 디바이스 사용 준비 */
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);

    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio); 	/* 해당 GPIO 디바이스의 방향 설정 */
    fd = open(buf, O_WRONLY);
    /* LED를 위해 디바이스를 출력으로 설정 : 입력의 경우 write(fd, “in”, 3); 사용 */
    write(fd, "out", 4);
    close(fd);

    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio); 	/* 디바이스에 값 출력 */
    fd = open(buf, O_WRONLY);
    write(fd, "1", 2);
    close(fd);

    getchar( ); /* LED 확인을 위한 대기 */

    fd = open("/sys/class/gpio/unexport", O_WRONLY); 		/* 사용한 GPIO 디바이스 해제하기 */
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);

    return 0;
}

int main(int argc, char **argv)
{
    int gno;

    if(argc < 2) {
        printf("Usage : %s GPIO_NO\n", argv[0]);
        return -1;
    }

    gno = atoi(argv[1]);
    ledControl(gno);

    return 0;
}
