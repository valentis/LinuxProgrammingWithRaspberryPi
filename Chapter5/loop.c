#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int i;

    /* 1초마다 0부터 숫자를 증가시키며 출력한다. */
    for(i = 0; ; i++) {
        printf("%10d\r", i);
        fflush(NULL);
        sleep(1);
    }

    return 0;
}
