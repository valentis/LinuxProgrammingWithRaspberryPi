#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

/* 키보드 입력을 처리하기 위한 함수 */
int kbhit(void)
{
    struct termios oldt, newt; 		/* 터미널에 대한 구조체 */
    int ch, oldf;

    tcgetattr(0, &oldt); 		/* 현재 터미널에 설정된 정보를 가져온다. */
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); 	/* 정규 모드 입력과 에코를 해제한다. */
    tcsetattr(0, TCSANOW, &newt); 	/* 새로운 값으로 터미널을 설정한다. */
    oldf = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, oldf | O_NONBLOCK); 	/* 입력을 넌블로킹 모드로 설정한다. */

    ch = getchar();

    tcsetattr(0, TCSANOW, &oldt); 	/* 기존의 값으로 터미널의 속성을 바로 적용한다. */
    fcntl(0, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin); 		/* 앞에서 읽으며 꺼낸 문자를 다시 넣는다. */
        return 1;
    }

    return 0;
}

int main(int argc, char** argv)
{
    int i = 0;
    for(i = 0; ; i++) {
        if(kbhit()) { 			/* 키보드가 눌러졌는지 확인한다. */
            switch(getchar()) { 	/* 문자를 읽는다. */
                case 'q': 		/* 읽은 문자가 q이면 종료한다. */
                    goto END;
                    break;
            };
        }

        printf("%20d\t\t\r", i); 	/* 현재 카운트한 숫자를 출력한다. */
        usleep(100); 			/* 100밀리초 동안 잠시 쉰다. */
    }

END:
    printf("Good Bye!\n");
    return 0;
}
