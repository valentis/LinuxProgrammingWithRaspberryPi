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

/* 선을 그린다. */
static void drawline(int fd, int start_x, int end_x, int y, ubyte r, ubyte g, ubyte b)
{
    ubyte a = 0xFF;

    /* for 루프를 이용해서 점을 이어서 선을 그린다. */
    for(int x = start_x; x < end_x; x++) {
        int offset = (x+y*vinfo.xres)*vinfo.bits_per_pixel/8.;
        lseek(fd, offset, SEEK_SET);
        write(fd, &b, 1);
        write(fd, &g, 1);
        write(fd, &r, 1);
        write(fd, &a, 1);
    }
}

/* 면을 그린다. */
static void drawface(int fd, int start_x, int start_y, int end_x, int end_y, 
                     ubyte r, ubyte g, ubyte b)
{
    ubyte a = 0xFF;
    if(end_x == 0) end_x = vinfo.xres;
    if(end_y == 0) end_y = vinfo.yres;

    /* 2개의 for 루프를 이용해서 면을 그린다. */
    for(int x = start_x; x < end_x; x++) {
        for(int y = start_y; y < end_y; y++) {
            int offset = (x + y*vinfo.xres)*vinfo.bits_per_pixel/8.;
            lseek(fd, offset, SEEK_SET);
            write(fd, &b, 1);
            write(fd, &g, 1);
            write(fd, &r, 1);
            write(fd, &a, 1);
        }
    }
}

/* 원을 그린다. */
static void drawcircle(int fd, int center_x, int center_y, int radius, 
	   	       ubyte r, ubyte g, ubyte b)
{
    int x = radius, y = 0;
    int radiusError = 1 - x;

    /* 순환문을 이용해서 원을 그리기: 정숫값을 계산해서 원 그리기 */
    while(x >= y) {
        drawpoint(fd, x + center_x, y + center_y, r, g, b);
        drawpoint(fd, y + center_x, x + center_y, r, g, b);
        drawpoint(fd, -x + center_x, y + center_y, r, g, b);
        drawpoint(fd, -y + center_x, x + center_y, r, g, b);
        drawpoint(fd, -x + center_x, -y + center_y, r, g, b);
        drawpoint(fd, -y + center_x, -x + center_y, r, g, b);
        drawpoint(fd, x + center_x, -y + center_y, r, g, b);
        drawpoint(fd, y + center_x, -x + center_y, r, g, b);
        y++;
        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}

int main(int argc, char** argv)
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

    drawface(fbfd, 0, 0, 0, 0, 255, 255, 0); 	/* 노란색(Yellow)을 생성 */
    drawcircle(fbfd, 200, 200, 100, 255, 0, 255); 	/* 자홍색(Magenta)을 생성 */
    drawline(fbfd, 0, 100, 200, 0, 255, 255); 	/* 청록색(Cyan)을 생성 */


    close(fbfd); 				/* 사용이 끝난 프레임 버퍼 디바이스를 닫는다. */

    return 0;
}
