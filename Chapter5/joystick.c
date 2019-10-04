#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>

#define JOY_DEV "/dev/input/event10"

int main()
{
    int fd;
    struct input_event ie;

    if((fd = open(JOY_DEV, O_RDONLY)) == -1) {
        perror("opening device");
        exit(EXIT_FAILURE);
    }

    while(read(fd, &ie, sizeof(struct input_event))) {
        printf("time %ld.%06ld\ttype %d\tcode %-3d\tvalue %d\n",
               ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);
        if(ie.type) {
            switch(ie.code) {
                case KEY_UP: printf("Up\n"); break; 		/* 'Up' 방향키: 조이스틱 'Up' */
                case KEY_DOWN: printf("Down\n"); break; 	/* 'Down' 방향키: 조이스틱 'Down' */
                case KEY_LEFT: printf("Left\n"); break; 	/* 'Left' 방향키: 조이스틱 'Left' */
                case KEY_RIGHT: printf("Right\n"); break; 	/* ‘Right’ 방향키: 조이스틱 ‘Right’ */
                case KEY_ENTER: printf("Enter\n"); break; 	/* ‘Enter’ 키: 조이스틱 ‘Press’ */
                default: printf("Default\n"); break; 		/* 위의 키가 아닌 경우 */
            }
        }
    }

    return 0;
}
