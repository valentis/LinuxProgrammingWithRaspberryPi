#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 			/* 저수준(Low level) I/O를 위해 사용 */
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/videodev2.h> 		/* Video4Linux2를 위한 헤더 파일 */

#include "bmpHeader.h"

#define VIDEODEV 	"/dev/video0" 	/* Pi Camera를 위한 디바이스 파일 */
#define FBDEV 		"/dev/fb0" 	/* 프레임 버퍼를 위한 디바이스 파일 */
#define WIDTH 		800 		/* 캡처할 영상의 크기 */
#define HEIGHT 		600
#define NUMCOLOR 	3

/* Video4Linux에서 사용할 영상을 저장하기 위한 버퍼를 위한 구조체 */
struct buffer {
    void* start;
    size_t length;
};

static int fd = -1; 			/* Pi Camera의 디바이스의 파일 디스크립터 */
struct buffer *buffers = NULL; 		/* Pi Camera의 MMAP를 위한 변수 */
static int fbfd = -1; 			/* 프레임 버퍼의 파일 디스크립터 */
static unsigned char *fbp = NULL; 	/* 프레임 버퍼의 MMAP를 위한 변수 */
static struct fb_var_screeninfo vinfo; 	/* 프레임 버퍼의 정보 저장을 위한 구조체 */

static void processImage(const void *p);
static int readFrame();
static void initRead(unsigned int buffer_size);

static void initDevice()
{
    struct v4l2_capability cap; 	/* 비디오 디바이스에 대한 기능을 조사한다. */
    struct v4l2_format fmt;
    unsigned int min;

    /* v4l2_capability 구조체를 이용해서 V4L2를 지원하는지 조사 */
    if(ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
        if(errno == EINVAL) {
            perror("/dev/video0 is no V4L2 device");
            exit(EXIT_FAILURE);
        } else {
            perror("VIDIOC_QUERYCAP");
            exit(EXIT_FAILURE);
        }
    }

    /* 카메라가 영상 캡처 기능이 있는지 조사한다. */
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        perror("/dev/video0 is no video capture device");
        exit(EXIT_FAILURE);
    }

    /* v4l2_format 구조체를 이용해서 영상의 포맷 설정 */
    //memset(&fmt, 0, sizeof(struct v4l2_format));
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    /* 카메라 디바이스에 영상의 포맷을 설정 */
    if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("VIDIOC_S_FMT");
        exit(EXIT_FAILURE);
    }

    /* 영상의 최소 크기를 구한다. */
    min = fmt.fmt.pix.width * vinfo.bits_per_pixel/8;
    if(fmt.fmt.pix.bytesperline < min) fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if(fmt.fmt.pix.sizeimage < min) fmt.fmt.pix.sizeimage = min;

    /* 영상 읽기를 위한 초기화 */
    initRead(fmt.fmt.pix.sizeimage);
}

static void initRead(unsigned int buffer_size)
{
    /* 카메라에서 사용하는 영상을 위한 메모리를 할당한다. */
    buffers = (struct buffer*)calloc(1, sizeof(*buffers));
    if(!buffers) {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }

    /* 버퍼를 초기화한다. */
    buffers[0].length = buffer_size;
    buffers[0].start = malloc(buffer_size);
    if(!buffers[0].start) {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }
}

void saveImage(unsigned char *inimg)
{
    RGBQUAD palrgb[256];
    FILE *fp;
    BITMAPFILEHEADER bmpFileHeader;
    BITMAPINFOHEADER bmpInfoHeader;

    /* BITMAPFILEHEADER 구조체에 BMP 파일 정보 설정 */
    memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
    bmpFileHeader.bfType = 0x4d42; /* (unsigned short)('B' | 'M' << 8)과 같다. */
    /* 54(14 + 40)바이트의 크기 */
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpFileHeader.bfOffBits += sizeof(RGBQUAD) * 256;
    bmpFileHeader.bfSize = bmpFileHeader.bfOffBits;
    bmpFileHeader.bfSize += WIDTH*HEIGHT*NUMCOLOR;

    /* BITMAPINFOHEADER 구조체에 BMP 이미지 정보 설정 */
    memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER); /* 40 바이트의 크기 */
    bmpInfoHeader.biWidth = WIDTH;
    bmpInfoHeader.biHeight = HEIGHT;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = NUMCOLOR*8;
    bmpInfoHeader.SizeImage = WIDTH*HEIGHT*bmpInfoHeader.biBitCount/8;
    bmpInfoHeader.biXPelsPerMeter = 0x0B12;
    bmpInfoHeader.biYPelsPerMeter = 0x0B12;

    /* 저장을 위한 이미지 파일 오픈 */
    if((fp = fopen("capture.bmp", "wb")) == NULL) {
        fprintf(stderr, "Error : Failed to open file...\n");
        exit(EXIT_FAILURE);
    }

    /* BMP 파일(BITMAPFILEHEADER) 정보 저장 */
    fwrite((void*)&bmpFileHeader, sizeof(bmpFileHeader), 1, fp);

    /* BMP 이미지(BITMAPINFOHEADER) 정보 저장 */
    fwrite((void*)&bmpInfoHeader, sizeof(bmpInfoHeader), 1, fp);

    /* 팔렛트(RGBQUAD) 정보 저장 */
    fwrite(palrgb, sizeof(RGBQUAD), 256, fp);

    /* BMP 데이터 저장 */
    fwrite(inimg, sizeof(unsigned char), WIDTH*HEIGHT*NUMCOLOR, fp);

    fclose(fp);
}

#define NO_OF_LOOP 1

static void mainloop()
{
    unsigned int count = NO_OF_LOOP;
    while(count-- > 0) { 		/* 100회 반복 */
        for(;;) {
            fd_set fds;
            struct timeval tv; 		/* select 함수의 대기시간을 위한 구조체 */
            int r;

            /* fd_set 구조체를 초기화하고 비디오 디바이스를 파일 디스크립터를 설정한다. */
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* 타임아웃을 2초로 설정한다. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            /* 비디오 데이터가 올 때까지 대기한다. */
            r = select(fd + 1, &fds, NULL, NULL, &tv);
            switch(r) {
                case -1: 		/* select( ) 함수 에러 시의 처리 */
                    if(errno != EINTR) {
                        perror("select()");
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 0: 		/* 타임아웃 시의 처리 */
                    perror("select timeout");
                    exit(EXIT_FAILURE);
                    break;
            };

            if(readFrame()) break; 	/* 카메라에서 하나의 프레임을 읽고 화면에 표시 */
        }
    }
}

static int readFrame()
{
     /* 카메라에서 하나의 프레임을 읽어온다. */
     if(read(fd, buffers[0].start, buffers[0].length) < 0) {
          perror("read()");
          exit(EXIT_FAILURE);
     }

     /* 읽어온 프레임을 색상 공간 등을 변경하고 화면에 출력한다. */
     processImage(buffers[0].start);

     return 1;
}

/* unsigned char의 범위를 넘어가지 않도록 경계 검사를 수행한다. */
inline unsigned char clip(int value, int min, int max);
unsigned char clip(int value, int min, int max)
{
    return(value > max ? max : value < min ? min : value);
}

/* YUYV를 BGRA로 변환한다. */
static void processImage(const void *p)
{
    int j, y;
    long location = 0, count = 0;
    int width = WIDTH, height = HEIGHT;
    int istride = WIDTH*2; 	/* 이미지의 폭을 넘어가면 다음 라인으로 내려가도록 설정한다. */
    unsigned char* in = (unsigned char*)p;
    int y0, u, y1, v, colors = vinfo.bits_per_pixel/8;
    unsigned char r, g, b, a = 0xff;
    unsigned char inimg[NUMCOLOR*WIDTH*HEIGHT]; /* 이미지 저장을 위한 변수 */

    for(y = 0; y < height; y++, in += istride, count = 0) {
        for(j = 0; j < vinfo.xres*2; j += colors) {
            if(j >= width*2) { 	/* 현재의 화면에서 이미지를 넘어서는 남은 공간을 처리한다. */
                location += colors*2;
                continue;
            }

            /* YUYV 성분을 분리한다. */
            y0 = in[j];
            u = in[j + 1] - 128;
            y1 = in[j + 2];
            v = in[j + 3] - 128;

            /* YUV를 RBGA로 전환한다. */
            r = clip((298 * y0 + 409 * v + 128) >> 8, 0, 255);
            g = clip((298 * y0 - 100 * u - 208 * v + 128) >> 8, 0, 255);
            b = clip((298 * y0 + 516 * u + 128) >> 8, 0, 255);
            fbp[location++] = b;
            fbp[location++] = g;
            fbp[location++] = r;
            fbp[location++] = a;

            /* BMP 이미지 데이터 */
            inimg[(height-y-1)*width*NUMCOLOR+count++] = b;
            inimg[(height-y-1)*width*NUMCOLOR+count++] = g;
            inimg[(height-y-1)*width*NUMCOLOR+count++] = r;

            /* YUV를 RBGA로 전환: Y1 */
            r = clip((298 * y1 + 409 * v + 128) >> 8, 0, 255);
            g = clip((298 * y1 - 100 * u - 208 * v + 128) >> 8, 0, 255);
            b = clip((298 * y1 + 516 * u + 128) >> 8, 0, 255);
            fbp[location++] = b;
            fbp[location++] = g;
            fbp[location++] = r;
            fbp[location++] = a;

            /* BMP 이미지 데이터 */
            inimg[(height-y-1)*width*NUMCOLOR+count++] = b;
            inimg[(height-y-1)*width*NUMCOLOR+count++] = g;
            inimg[(height-y-1)*width*NUMCOLOR+count++] = r;
        }
    }

    /* 이미지 데이터를 BMP 파일로 저장한다. */
    saveImage(inimg);
}

static void uninitDevice()
{
    long screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8;

    /* 사용했던 메모리를 해제한다. */
    free(buffers[0].start);
    free(buffers);
    munmap(fbp, screensize);
}

int main(int argc, char **argv)
{
    long screensize = 0;

    /* 디바이스 열기 */
    /* Pi Camera 열기 */
    fd = open(VIDEODEV, O_RDWR | O_NONBLOCK, 0);
    if(fd == -1) {
        perror("open() : video devive");
        return EXIT_FAILURE;
    }

    /* 프레임 버퍼 열기 */
    fbfd = open(FBDEV, O_RDWR);
    if(fbfd == -1) {
        perror("open() : framebuffer device");
        return EXIT_FAILURE;
    }

    /* 프레임 버퍼의 정보 가져오기 */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information.");
        exit(EXIT_FAILURE);
    }

    /* mmap(): 프레임 버퍼를 위한 메모리 공간 확보 */
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8;
    fbp = (unsigned char *)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if((int)fbp == -1) {
        perror("mmap() : framebuffer device to memory");
        return EXIT_FAILURE;
    }
    memset(fbp, 0, screensize);

    initDevice(); 				/* 디바이스 초기화 */
    mainloop(); 				/* 캡처 실행 */
    uninitDevice(); 				/* 디바이스 해제 */

    /* 디바이스 닫기 */
    close(fbfd);
    close(fd);

    return EXIT_SUCCESS; 			/* 애플리케이션 종료 */
}
