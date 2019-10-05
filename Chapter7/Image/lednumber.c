#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define FBDEVICE "/dev/fb1" /* SenseHAT의 8×8 LED 매트릭스의 디바이스 파일 */
                                
/* 색상을 24비트에서 16비트로 변경한다. */
extern inline unsigned short makepixel(unsigned char r, unsigned char g, unsigned char b)
{
    return (unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));
}

int main(int argc, char **argv)
{
    struct fb_fix_screeninfo finfo; /* 프레임 버퍼의 고정 정보를 위한 구조체 */
    struct fb_var_screeninfo vinfo; /* 프레임 버퍼의 가변 정보를 위한 구조체 */
    unsigned short *pfb;
    int fbfd, size, num;
    unsigned short c = makepixel(127, 0, 127);
    short number[11][64] = {{0, 0, c, c, c, c, 0, 0, /* 0 */ \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, c, c, c, 0, \
                             0, c, c, c, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, 0, c, c, c, c, 0, 0},
                            {0, 0, 0, c, c, 0, 0, 0, /* 1 */ \
                             0, 0, c, c, c, 0, 0, 0, \
                             0, 0, c, c, c, 0, 0, 0, \
                             0, 0, 0, c, c, 0, 0, 0, \
                             0, 0, 0, c, c, 0, 0, 0, \
                             0, 0, 0, c, c, 0, 0, 0, \
                             0, 0, 0, c, c, 0, 0, 0, \
                             0, 0, c, c, c, c, 0, 0}, \
                            {0, 0, c, c, c, c, 0, 0, /* 2 */ \
                             0, c, c, c, c, c, c, 0, \
                             0, c, 0, 0, 0, c, c, 0, \
                             0, 0, 0, 0, c, c, 0, 0, \
                             0, 0, c, c, 0, 0, 0, 0, \
                             0, c, c, 0, 0, 0, 0, 0, \
                             0, c, c, c, c, c, c, 0, \
                             0, 0, c, c, c, c, 0, 0}, \
                            {0, 0, c, c, c, c, 0, 0, /* 3 */ \
                             0, c, c, 0, 0, c, c, 0, \
                             0, 0, 0, 0, 0, c, c, 0, \
                             0, 0, 0, c, c, c, 0, 0, \
                             0, 0, 0, c, c, c, c, 0, \
                             0, 0, 0, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, 0, c, c, c, c, 0, 0}, \
                            {0, 0, 0, 0, c, c, 0, 0, /* 4 */ \
                             0, 0, 0, c, c, c, 0, 0, \
                             0, 0, c, 0, c, c, 0, 0, \
                             0, c, 0, 0, c, c, 0, 0, \
                             0, c, c, c, c, c, c, 0, \
                             0, c, c, c, c, c, c, 0, \
                             0, 0, 0, 0, c, c, 0, 0, \
                             0, 0, 0, 0, c, c, 0, 0}, \
                            {0, c, c, c, c, c, c, 0, /* 5 */ \
                             0, c, c, 0, 0, 0, 0, 0, \
                             0, c, c, 0, 0, 0, 0, 0, \
                             0, c, c, c, c, c, 0, 0, \
                             0, 0, 0, 0, 0, c, c, 0, \
                             0, c, 0, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, 0, c, c, c, c, 0, 0}, \
                            {0, 0, c, c, c, c, 0, 0, /* 6 */ \
                             0, c, c, 0, 0, 0, c, 0, \
                             0, c, c, 0, 0, 0, 0, 0, \
                             0, c, c, c, c, c, 0, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, 0, c, c, c, c, 0, 0}, \
                            {0, c, c, c, c, c, c, 0, /* 7 */ \
                             0, c, c, 0, 0, c, c, 0, \
                             0, 0, 0, 0, 0, c, c, 0, \
                             0, 0, 0, 0, c, c, 0, 0, \
                             0, 0, 0, c, c, 0, 0, 0, \
                             0, 0, 0, c, c, 0, 0, 0, \
                             0, 0, 0, c, c, 0, 0, 0, \
                             0, 0, 0, c, c, 0, 0, 0}, \
                            {0, 0, c, c, c, c, 0, 0, /* 8 */ \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, c, 0, c, c, 0, \
                             0, 0, c, c, c, c, 0, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, 0, c, c, c, c, 0, 0}, \
                            {0, 0, c, c, c, c, 0, 0, /* 9 */ \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, c, c, 0, 0, c, c, 0, \
                             0, 0, c, c, c, c, c, 0, \
                             0, 0, 0, 0, 0, c, c, 0, \
                             0, c, 0, 0, 0, c, c, 0, \
                             0, 0, c, c, c, c, 0, 0}, \
                            {0, 0, 0, 0, 0, 0, 0, 0, /* 10 */ \
                             0, 0, 0, 0, 0, 0, 0, 0, \
                             0, 0, 0, 0, 0, 0, 0, 0, \
                             0, 0, 0, 0, 0, 0, 0, 0, \
                             0, 0, 0, 0, 0, 0, 0, 0, \
                             0, 0, 0, 0, 0, 0, 0, 0, \
                             0, 0, 0, 0, 0, 0, 0, 0, \
                             0, 0, 0, 0, 0, 0, 0, 0}
                        };

    if(argc < 2) {
        printf("Usage : %s number\n", argv[0]);
        return EXIT_FAILURE;
    }

    num = atoi(argv[1]);

    fbfd = open(FBDEVICE, O_RDWR); /* 사용할 프레임 버퍼 디바이스를 연다. */
    if(fbfd < 0) {
        perror("Error: cannot open framebuffer device");
        return EXIT_FAILURE;
    }

    ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo); /* 프레임 버퍼의 고정 정보를 가져온다. */
    if (strcmp(finfo.id, "RPi-Sense FB") != 0) { /* 현재 장치가 SenseHAT인지 확인한다. */
        printf("%s＼n", "Error: RPi-Sense FB not found");
        close(fbfd);
        return EXIT_FAILURE;
    }

    /* 현재 프레임 버퍼에 대한 화면 정보를 얻어온다. */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return EXIT_FAILURE;
    }

    size = vinfo.xres * vinfo.yres * sizeof(short);

    /* 가져온 화면 정보로 버퍼의 크기를 계산한다. */
    /* 프레임 버퍼를 메모리로 매핑(사상)한다. */
    pfb = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (pfb == MAP_FAILED) {
        close(fbfd);
        perror("Error mmapping the file");
        return EXIT_FAILURE;
    }

    /* SenseHAT의 8×8 LED 매트릭스를 숫자 표시 */
    memcpy(pfb, number[num], size);

    /* 메모리 매핑을 해제하고 장치를 닫는다. */
    if (munmap(pfb, size) == -1) {
        perror("Error un-mmapping the file");
    }

    close(fbfd);

    return EXIT_SUCCESS;
}
