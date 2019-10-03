#include <wiringPi.h>
#include <softTone.h>

#define SPKR 	6 	/* GPIO25 */
#define TOTAL 	32 	/* 학교종의 전체 계이름의 수 */

int notes[] = { 	/* 학교종을 연주하기 위한 계이름 */
	391, 391, 440, 440, 391, 391, 329.63, 329.63, \
	391, 391, 329.63, 329.63, 293.66, 293.66, 293.66, 0, \
	391, 391, 440, 440, 391, 391, 329.63, 329.63, \
	391, 329.63, 293.66, 329.63, 261.63, 261.63, 261.63, 0
};

int musicPlay( )
{
    int i;
    softToneCreate(SPKR); 	/* 톤 출력을 위한 GPIO 설정 */
    for (i = 0; i < TOTAL; ++i) {
        softToneWrite(SPKR, notes[i]); /* 톤 출력 : 학교종 연주 */
        delay(280); 		/* 음의 전체 길이만큼 출력되도록 대기 */
    }
    return 0;
}

int main( )
{
    wiringPiSetup( );
    musicPlay( ); 		/* 음악 연주를 위한 함수 호출 */
    return 0;
}
