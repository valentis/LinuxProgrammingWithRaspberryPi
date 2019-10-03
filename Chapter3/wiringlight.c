#include <wiringPi.h>
#include <stdio.h>

#define SW 	5 	/* GPIO24 */
#define CDS 	0 	/* GPIO17 */
#define LED 	1 	/* GPIO18 */

int cdsControl( )
{
    int i;

    pinMode(SW, INPUT); 	/* Pin 모드를 입력으로 설정 */
    pinMode(CDS, INPUT); 	/* Pin 모드를 입력으로 설정 */
    pinMode(LED, OUTPUT); 	/* Pin 모드를 출력으로 설정 */

    for (;;) { 			/* 조도 센서 검사를 위해 무한 루프를 실행한다. */
        if(digitalRead(CDS) == HIGH) { 	/* 빛이 감지되면(HIGH) */
            digitalWrite(LED, HIGH); 	/* LED 켜기(On) */
            delay(1000);
            digitalWrite(LED, LOW); 	/* LED 끄기(Off) */
        }
    }

    return 0;
}

int main( )
{
    wiringPiSetup( );
    cdsControl( ); 		/* 조도 센서 사용을 위한 함수 호출 */
    return 0;
}
