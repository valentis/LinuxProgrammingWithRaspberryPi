#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>                                    	/* Low level I/O를 위해서 사용 */
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>                                  /* <videodev2.h> 파일을 위해서 사용 */
#include <linux/videodev2.h>                            /* Video4Linux2를 위한 헤더파일 */
#include <linux/fb.h>
#include <arm_neon.h>

#define MEMZERO(x) memset(&(x), 0, sizeof(x))    	/* 변수 초기화를 위한 매크로 */

#define VIDEODEV        "/dev/video0"               	/* Pi Camera를 위한 디바이스 파일 */
#define FBDEV           "/dev/fb0"                    	/* 프레이버퍼를 위한 디바이스 파일 */
#define WIDTH           800                             /* 캡쳐받을 영상의 크기 */
#define HEIGHT          600                

typedef unsigned char ubyte;

/* Video4Linux에서 사용할 영상 저장을 위한 버퍼 */
struct buffer {
    void * start;
    size_t length;
};

static int fd = -1;                                   	/* Pi Camera의 장치의 파일 디스크립터 */
struct buffer *buffers = NULL;                        	/* Pi Camera의MMAP를 위한 변수 */
static int fbfd = -1;                                   /* 프레임버퍼의 파일 디스크립터 */
static ubyte *fbp = NULL;                            	/* 프레임버퍼의 MMAP를 위한 변수 */
static struct fb_var_screeninfo vinfo;                  /* 프레임버퍼의 정보 저장을 위한 구조체 */

static void processImage(const void *p);
static int readFrame();
static void initRead(unsigned int buffer_size);

static void initDevice()
{   
    struct v4l2_capability cap;                         /* 비디오 장치에 대한 기능을 조사한다. */
    struct v4l2_format fmt;
    unsigned int min;

    if(ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {       	/* 장치가 V4L2를 지원하는지 조사 */
        if(errno == EINVAL) {
            perror("/dev/video0 is no V4L2 device");
            exit(EXIT_FAILURE);
        } else {
            perror("VIDIOC_QUERYCAP");
            exit(EXIT_FAILURE);
        }
    }

    /* 장치가 영상 캡쳐 기능이 있는지 조사 */
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        perror("/dev/video0 is no video capture device");
        exit(EXIT_FAILURE);
    }

    MEMZERO(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("VIDIOC_S_FMT");
        exit(EXIT_FAILURE);
    }

    /* 영상의 최소 크기를 구함 */
    min = fmt.fmt.pix.width * vinfo.bits_per_pixel/8;
    if(fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if(fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    /* 영상 읽기를 위한 초기화 */
    initRead(fmt.fmt.pix.sizeimage); 
}

static void initRead(unsigned int buffer_size) 
{
    /* 메모리를 할당한다 */
    buffers = (struct buffer*)calloc(1, sizeof(*buffers));
    if(!buffers) {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }

    /* 버퍼를 초기화 한다. */
    buffers[0].length = buffer_size;
    buffers[0].start = malloc(buffer_size);
    if(!buffers[0].start) {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }
} 

#define NO_OF_LOOP    100

static void mainloop() 
{
    unsigned int count = NO_OF_LOOP;
    while(count-- > 0) {                                /* 100회 반복 */
        for(;;) {
            fd_set fds;
            struct timeval tv;
            int r;

            /* fd_set 구조체를 초기화하고 비디오 장치를 파일 디스크립터를 설정한다. */
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* 타임아웃(Timeout)을 2초로 설정한다. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            r = select(fd + 1, &fds, NULL, NULL, &tv);  /* 비디오 데이터가 올때까지 대기 */
            switch(r) {
                case -1:                                /* select( ) 함수 에러시의 처리 */
                      if(errno != EINTR) {
                          perror("select( )");
                          exit(EXIT_FAILURE);
                      }
                      break;
                case 0:                                 /* 타임아웃시의 처리 */
                      perror("select timeout");
                      exit(EXIT_FAILURE);
                      break;
            };

            if(readFrame()) break;                      /* 현재의 프레임을 읽어서 표시 */
        }
    }
}

static int readFrame( ) 
{
  if(read(fd, buffers[0].start, buffers[0].length) < 0) {
     perror("read( )");
     exit(EXIT_FAILURE);
  }

  processImage(buffers[0].start);

  return 1;
}

/* unsigned char의 범위를 넘어가지 않도록 경계 검사를 수행다. */
ubyte clip(int value, int min, int max)
{
    return(value > max ? max : value < min ? min : value);
}

/* YUYV를 BGRA로 변환한다. */
static void processImage(const void *p)
{
    const int width = WIDTH, height = HEIGHT;
    const uint8_t colorbit = vinfo.bits_per_pixel;
    ubyte* in = (ubyte*)p;
    int16x8_t const half = vdupq_n_s16(128);

    for(int i = 0, j = 0; i < width*height*2; i+=colorbit, j+=2*colorbit) { 
        if(!(i%(width*2)) && i) {         		/* 현재의 화면에서 이미지를 넘어서는 빈 공간을 처리 */
            j+=(vinfo.xres-width)*colorbit/8;
            continue;
        }
        const ubyte *data = in+i;

        /* YUYV 성분을 분리 */
        uint8x8x4_t yuyv = vld4_u8(data);
        int16x8_t y0 = (int16x8_t)vmovl_u8(yuyv.val[0]);
        int16x8_t u = vsubq_s16((int16x8_t)vmovl_u8(yuyv.val[1]), half);
        int16x8_t y1 = (int16x8_t)vmovl_u8(yuyv.val[2]);
        int16x8_t v = vsubq_s16((int16x8_t)vmovl_u8(yuyv.val[3]), half);

        /* Blue : (298 * y + 516 * u + 128) >> 8 */
        int32x4_t bL = vshrq_n_s32(vmull_n_s16(vget_low_s16(u) , 512), 8);
        int32x4_t bH = vshrq_n_s32(vmull_n_s16(vget_high_s16(u), 512), 8);

        /* Green : (298 * y - 100 * u - 208 * v + 128) >> 8 */
        int32x4_t gL = vshrq_n_s32(vaddq_s32(vmull_n_s16(vget_low_s16(u) , -100), 
                                    vmull_n_s16(vget_low_s16(v) , -208)), 8);
        int32x4_t gH = vshrq_n_s32(vaddq_s32(vmull_n_s16(vget_high_s16(u), -100), 
                                     vmull_n_s16(vget_high_s16(v), -208)), 8);

        /* Red : (298 * y + 409 * v + 128) >> 8 */
        int32x4_t rL = vshrq_n_s32(vmull_n_s16(vget_low_s16(v) , 409), 8);
        int32x4_t rH = vshrq_n_s32(vmull_n_s16(vget_high_s16(v), 409), 8);

        /* Y0 요소의 처리 */
        int32x4_t y0L = vmovl_s16(vget_low_s16(y0));
        int32x4_t y0H = vmovl_s16(vget_high_s16(y0));

        /* Y1 요소의 처리 */
        int32x4_t y1L = vmovl_s16(vget_low_s16(y1));
        int32x4_t y1H = vmovl_s16(vget_high_s16(y1));

        /* Y0 & UV 데이터 병합 */
        uint8x8_t c0 = vmovn_u16(vcombine_u16(vqmovun_s32(vaddq_s32(y0L, bL)), 
                                    vqmovun_s32(vaddq_s32(y0H, bH))));
        uint8x8_t c1 = vmovn_u16(vcombine_u16(vqmovun_s32(vaddq_s32(y0L, gL)), 
                                    vqmovun_s32(vaddq_s32(y0H, gH))));
        uint8x8_t c2 = vmovn_u16(vcombine_u16(vqmovun_s32(vaddq_s32(y0L, rL)), 
                                    vqmovun_s32(vaddq_s32(y0H, rH))));

        /* Y1 & UV 데이터 병합 */
        uint8x8_t c3 = vmovn_u16(vcombine_u16(vqmovun_s32(vaddq_s32(y1L, bL)), 
                                    vqmovun_s32(vaddq_s32(y1H, bH))));
        uint8x8_t c4 = vmovn_u16(vcombine_u16(vqmovun_s32(vaddq_s32(y1L, gL)), 
                                    vqmovun_s32(vaddq_s32(y1H, gH))));
        uint8x8_t c5 = vmovn_u16(vcombine_u16(vqmovun_s32(vaddq_s32(y1L, rL)), 
                                    vqmovun_s32(vaddq_s32(y1H, rH)))); 

        /* 색상 벡터를 프레임버퍼로 복사 */
        uint16x8x4_t pblock;
        uint8x8_t alpha = vmov_n_u8(255); 
        uint8x8_t nc = vcgt_u8(c3, vmov_n_u8(64));			/* Red의 값이 64보다 크면 255, 아니면 0 */

        pblock.val[0] = vaddw_u8(vshll_n_u8(c1 , 8), c0);
        pblock.val[1] = vaddw_u8(vshll_n_u8(alpha, 8), nc);
        pblock.val[2] = vaddw_u8(vshll_n_u8(c1 , 8), c0);
        pblock.val[3] = vaddw_u8(vshll_n_u8(alpha, 8), nc);
        vst4q_u16((uint16_t *)(fbp+j), pblock);
    }
}

static void uninitDevice() 
{
    long screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8;

    free(buffers[0].start);
    free(buffers);
    munmap(fbp, screensize);
}

int main(int argc, char ** argv)
{
    long screensize = 0;

    /* 장치 열기 */
    /* Pi Camera 열기 */
    fd = open(VIDEODEV, O_RDWR| O_NONBLOCK, 0);
    if(fd == -1) {
        perror("open( ) : video devive");
        return EXIT_FAILURE;
    }

    /* 프레임버퍼 열기 */
    fbfd = open(FBDEV, O_RDWR);
    if(fbfd == -1) {
        perror("open( ) : framebuffer device");
        return EXIT_FAILURE;
    }

    /* 프레임버퍼의 정보 가져오기 */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
         perror("Error reading variable information.");
         exit(EXIT_FAILURE);
    }

    /* mmap( ) : 프레임버퍼를 위한 메모리 공간 확보 */
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8;
    fbp = (ubyte *)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if((int)fbp == -1) {
        perror("mmap() : framebuffer device to memory");
        return EXIT_FAILURE;
    }
    memset(fbp, 0, screensize);

    initDevice();                                       /* 장치 초기화 */ 
    mainloop();                                         /* 캡쳐 실행 */
    uninitDevice();                                     /* 장치 해제 */

    /* 장치 닫기 */
    close(fbfd);
    close(fd);

    return EXIT_SUCCESS;                                /* 애플리케이션 종료 */
}

