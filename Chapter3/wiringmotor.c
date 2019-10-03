#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int motorControl(int gpio)
{
    int i;
    pinMode(gpio, OUTPUT); 		/* Pin의 모드 설정 */
    for (i = 0; i < 3; i++) {
        digitalWrite(gpio, HIGH); 	/* HIGH(1) 값을 출력 : 모터 켜기 */
        delay(1000); 			/* 1초(1000밀리초) 동안 대기 */
        digitalWrite(gpio, LOW); 	/* LOW(0) 값을 출력 : 모터 끄기 */
        delay(1000);
    }
    return 0;
}

int main(int argc, char** argv)
{
    int gno;
    if(argc < 2) {
        printf("Usage : %s GPIO_NO\n", argv[0]);
        return -1;
    }

    gno = atoi(argv[1]);
    wiringPiSetup( );
    motorControl(gno);

    return 0;
}
