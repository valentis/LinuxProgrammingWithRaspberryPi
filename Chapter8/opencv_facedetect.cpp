#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#define FBDEV "/dev/fb0"
#define CAMERA_COUNT 3
#define CAM_WIDTH 640
#define CAM_HEIGHT 480

using namespace cv;

const static char* cascade_name = \
       "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";

int main(int argc, char **argv)
{
    int fbfd;

    /* 프레임 버퍼 정보 처리를 위한 구조체 */
    struct fb_var_screeninfo vinfo;
    unsigned char *buffer, *pfbmap;
    unsigned int x, y, i, j, screensize;
    VideoCapture vc(0); 					/* 카메라를 위한 변수 */
    CascadeClassifier cascade; 					/* 얼굴 인식을 위한 classifier */
    Mat frame(CAM_WIDTH, CAM_HEIGHT, CV_8UC3, Scalar(255)); 	/* 영상을 위한 변수 */
    Point pt1, pt2; 						/* 인식된 얼굴의 대각선의 두 점을 저장하기 위한 변수들 */

    /* 얼굴 인식을 위한 캐스케이드를 불러온다. */
    if(!cascade.load(cascade_name)) {
        perror("load()");
        return EXIT_FAILURE;
    }

    /* 캡처할 영상의 속성을 설정한다. */
    vc.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
    vc.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);

    /* 프레임 버퍼 열기 */
    fbfd = open(FBDEV, O_RDWR);
    if(fbfd == -1) {
        perror("open() : framebuffer device");
        return EXIT_FAILURE;
    }

    /* 프레임 버퍼의 정보 가져오기 */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information.");
        return EXIT_FAILURE;
    }

    /* mmap(): 프레임 버퍼를 위한 메모리 공간 확보 */
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8.;
    pfbmap = (unsigned char *)mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if((int)pfbmap == -1) {
        perror("mmap() : framebuffer device to memory");
        return EXIT_FAILURE;
    }

    memset(pfbmap, 0, screensize);
    for(i = 0; i < CAMERA_COUNT; i++) {
        int colors = vinfo.bits_per_pixel/8;
        long location = 0;
        int istride = frame.cols*colors; 			/* 이미지의 폭을 넘어가면 다음 라인으로 내려가도록 설정 */
        vc >> frame; 						/* 카메라로부터 한 프레임의 영상을 가져온다. */

        /* 카메라 캡처를 위해 영상을 다른 이름으로 복사한다. */
        Mat image(CAM_WIDTH, CAM_HEIGHT, CV_8UC1, Scalar(255));
        cvtColor(frame, image, CV_BGR2GRAY);
        equalizeHist(image, image);

        /* 영상에서 얼굴들을 인식한다. */
        std::vector<Rect> faces;
        cascade.detectMultiScale(image, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

        /* 영상에서 찾은 얼굴들을 프레임에 표시한다. */
        for(j = 0; j < faces.size(); j++) {
            /* 찾은 얼굴의 양 모서리 구하기 */
            pt1.x = faces[j].x; pt2.x = (faces[j].x + faces[j].width);
            pt1.y = faces[j].y; pt2.y = (faces[j].y + faces[j].height);

            /* 이미지에 얼굴 그리기 */
            rectangle(frame, pt1, pt2, Scalar(255,0,0), 3, 8);
        }

        buffer = (uchar*)frame.data; 				/* Mat 클래스의 영상 데이터 획득 */

        /* 프레임 버퍼로 출력 */
        for(y = 0, location = 0; y < frame.rows; y++) {
            for(x = 0; x < vinfo.xres; x++) {
                /* 화면에서 이미지를 넘어서는 빈 공간을 처리 */
                if(x >= frame.cols) {
                    location+=colors;
                    continue;
                }

                pfbmap[location++] = *(buffer+(y*frame.cols+x)*3+0);
                pfbmap[location++] = *(buffer+(y*frame.cols+x)*3+1);
                pfbmap[location++] = *(buffer+(y*frame.cols+x)*3+2);
                pfbmap[location++] = 0xFF;
            }
        }
    }

    /* 사용이 끝난 자원과 메모리를 해제한다. */
    munmap(pfbmap, screensize);

    close(fbfd);

    return 0;
}
