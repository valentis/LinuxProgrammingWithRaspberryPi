#include <wiringPi.h>
#include <stdio.h>

#define SW 	5 		/* GPIO24 */
#define LED 	1 		/* GPIO18 */

int switchControl( )
{
    int i;

    pinMode(SW, INPUT); 	/* Pin 모드를 입력으로 설정 */
    pinMode(LED, OUTPUT);

    for (;;) { 			/* 스위치의 확인을 위해 무한 루프를 수행한다. */
        if(digitalRead(SW) == LOW) { 	/* Push 버튼이 눌러지면(LOW) */
            digitalWrite(LED, HIGH); 	/* LED 켜기(On) */
            delay(1000);
            digitalWrite(LED, LOW); 	/* LED 끄기(Off) */
        }
     delay(10); 		/* Ctrl+C 등의 이벤트를 감지하기 위해 잠시 쉰다. */
    }

    return 0;
}

int main(int argc, char **argv)
{
    wiringPiSetup( );
    switchControl( ); 		/* 스위치 사용을 위한 함수 */
    return 0;
}
