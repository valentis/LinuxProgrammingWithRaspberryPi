#include <stdio.h>
#include <stdlib.h> /* malloc() 함수와 free() 함수를 위해서 사용한다. */
#include <string.h> /* memcpy() 함수를 위해서 사용한다. */
#include <fcntl.h> /* O_RDWR 상수를 위해서 사용한다. */
#include <limits.h> /* USHRT_MAX 상수를 위해서 사용한다. */
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <arm_neon.h>                    /* ARM NEON 프로그래밍을 위한 헤더 파일 */

#include "bmpHeader.h"

#define FBDEVFILE "/dev/fb0"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) (n>UCHAR_MAX)?UCHAR_MAX:(n<0)?0:n

typedef unsigned char ubyte;

/* BMP 파일의 헤더를 분석해서 원하는 정보를 얻기 위한 함수 */
extern int readBmp(char *filename, ubyte **pData, int *cols, int *rows, int *color);

/* BGR 이미지 데이터를 그레이스케일로 변환하는 함수 */
void cvtBGR2Gray(ubyte *dest, ubyte *src, int n)
{
    uint8x8_t bfac = vdup_n_u8(77);
    uint8x8_t gfac = vdup_n_u8(151);
    uint8x8_t rfac = vdup_n_u8(28);
    n/=8;
    for(int i = 0; i < n; i++) {
        uint8x8x3_t bgr = vld3_u8(src);
        uint8x8x3_t gray;
        uint8x8_t result;
        uint16x8_t temp;
        src  += 8*3;

        temp = vmull_u8(bgr.val[0],       bfac);
        temp = vmlal_u8(temp, bgr.val[1], gfac);
        temp = vmlal_u8(temp, bgr.val[2], rfac);
        result = vshrn_n_u16(temp, 8);
#if 0
        vst1_u8(dest, result);
        dest += 8;
#else
        gray.val[0] = result;
        gray.val[1] = result;
        gray.val[2] = result;
        vst3_u8(dest, gray);
        dest += 8*3;
#endif
    }
}

int main(int argc, char **argv)
{
    int cols, rows, color = 24; /* 프레임 버퍼의 가로 × 세로의 크기 */
    ubyte r, g, b, a = 255;
    ubyte *pData, *pBmpData, *pImageData, *pFbMap;
    struct fb_var_screeninfo vinfo;
    int fbfd;

    if(argc != 2) {
        printf("Usage: ./%s xxx.bmp\n", argv[0]);
        return 0;
    }

    /* 프레임 버퍼를 연다. */
    fbfd = open(FBDEVFILE, O_RDWR);
    if(fbfd < 0) {
        perror("open()");
        return -1;
    }

    /* 현재 프레임 버퍼에 대한 화면 정보를 얻어온다. */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("ioctl() : FBIOGET_VSCREENINFO");
        return -1;
    }

    /* BMP 출력을 위한 변수의 메모리 할당 */
    pBmpData = (ubyte *)malloc(vinfo.xres * vinfo.yres * sizeof(ubyte) * vinfo.bits_per_pixel/8);
    pData = (ubyte*)malloc(vinfo.xres * vinfo.yres * sizeof(ubyte) * color/8);
    pImageData = (ubyte *)malloc(vinfo.xres * vinfo.yres * sizeof(ubyte) * color/8);

    /* 프레임 버퍼에 대한 메모리 맵을 수행한다. */
    pFbMap = (ubyte *)mmap(0, vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);
    if((unsigned)pFbMap == (unsigned)-1) {
        perror("mmap()");
        return -1;
    }

    /* BMP 파일에서 헤더 정보를 가져온다. */
    if(readBmp(argv[1], &pImageData, &cols, &rows, &color) < 0) {
        perror("readBmp()");
        return -1;
    }

    cvtBGR2Gray(pData, pImageData, vinfo.xres*vinfo.yres);

    /* BMP 이미지 데이터를 프레임 버퍼 데이터로 변경 */
    for(int y = 0, k, total_y; y < rows; y++) {
        k = (rows-y-1)*cols*color/8;
        total_y = y*vinfo.xres*vinfo.bits_per_pixel/8;
        for(int x = 0; x < cols; x++) {
            /* BMP 이미지는 뒤집혀 있기 때문에 BGR 형태로 가져온다. */
            b = LIMIT_UBYTE(pData[k+x*color/8+0]);
            g = LIMIT_UBYTE(pData[k+x*color/8+1]);
            r = LIMIT_UBYTE(pData[k+x*color/8+2]);
            *(pBmpData + x*vinfo.bits_per_pixel/8 + total_y + 0) = b;
            *(pBmpData + x*vinfo.bits_per_pixel/8 + total_y + 1) = g;
            *(pBmpData + x*vinfo.bits_per_pixel/8 + total_y + 2) = r;
            *(pBmpData + x*vinfo.bits_per_pixel/8 + total_y + 3) = a;
        }
    }

    /* 앞에서 생성한 BMP 데이터를 프레임 버퍼의 메모리 공간으로 복사 */
    memcpy(pFbMap, pBmpData, vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8);

    /* 프레임 버퍼 파일을 닫고 이미지 데이터를 위해서 사용한 메모리를 해제 */
    munmap(pFbMap, vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8);

    free(pBmpData);
    free(pImageData);
    free(pData);

    close(fbfd); /* 프레임 버퍼를 위한 디바이스 파일 닫기 */

    return 0;
}
