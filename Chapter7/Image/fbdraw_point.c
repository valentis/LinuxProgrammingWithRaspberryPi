#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define FBDEVICE "/dev/fb0"

typedef unsigned char ubyte;
struct fb_var_screeninfo vinfo;

/* 점을 그린다. */
static void drawpoint(int fd, int x, int y, ubyte r, ubyte g, ubyte b)
{
    ubyte a = 0xFF;

    /* 색상 출력을 위한 위치를 구한다. */
    /* offset = (X의_위치 + Y의_위치 × 해상도의_넓이) × 색상의_바이트_수 */
    int offset = (x + y*vinfo.xres)*vinfo.bits_per_pixel/8.;
    lseek(fd, offset, SEEK_SET);
    write(fd, &b, 1);
    write(fd, &g, 1);
    write(fd, &r, 1);
    write(fd, &a, 1);
}

int main(int argc, char **argv)
{
    int fbfd, status, offset;

    /* 프레임 버퍼 정보 처리를 위한 구조체 */
    unsigned short pixel;
    fbfd = open(FBDEVICE, O_RDWR); 		/* 사용할 프레임 버퍼 디바이스를 연다. */
    if(fbfd < 0) {
        perror("Error: cannot open framebuffer device");
        return -1;
    }

    /* 현재 프레임 버퍼에 대한 화면 정보를 얻어온다. */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }

    drawpoint(fbfd, 50, 50, 255, 0, 0); 	/* 빨간색(Red) 점을 출력 */
    drawpoint(fbfd, 100, 100, 0, 255, 0); 	/* 초록색(Green) 점을 출력 */
    drawpoint(fbfd, 150, 150, 0, 0, 255); 	/* 파란색(Blue) 점을 출력 */

    close(fbfd); 				/* 사용이 끝난 프레임 버퍼 디바이스를 닫는다. */

    return 0;
}
