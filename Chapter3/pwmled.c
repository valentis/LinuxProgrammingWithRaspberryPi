#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>

void ledPwmControl(int gpio)
{
    pinMode(gpio, OUTPUT); 		/* Pin의 출력 설정 */
    softPwmCreate(gpio, 0, 255); 	/* PWM의 범위 설정 */

    for(int i = 0; i < 10000; i++) {
        softPwmWrite(gpio, i&255); 	/* PWM 값을 출력: LED 켜기 */
        delay(5);
    }

    softPwmWrite(gpio, 0); 		/* LED 끄기 */
}

int main(int argc, char** argv)
{
    int gno;

    if(argc < 2) {
        printf("Usage : %s GPIO_NO\n", argv[0]);
        return -1;
    }

    gno = atoi(argv[1]);
    wiringPiSetup( ); 			/* wiringPi 초기화 */
    ledPwmControl(gno);

    return 0;
}
