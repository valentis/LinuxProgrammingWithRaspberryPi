#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>

#pragma comment(lib, "cv.lib")
#pragma comment(lib, "highgui.lib")
#pragma comment(lib, "cxcore.lib")

int main(int argc, char **argv)
{
    IplImage *image1 = cvLoadImage("sample1.jpg", CV_LOAD_IMAGE_COLOR);
    IplImage *image2 = cvLoadImage("sample2.jpg", CV_LOAD_IMAGE_COLOR);
    IplImage *image_add = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 3);
    IplImage *image_sub = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 3);
    IplImage *image_mul = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 3);
    IplImage *image_div = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 3);
    IplImage *image_gray1 = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
    IplImage *image_gray2 = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
    IplImage *image_white = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
    IplImage *image_gray_sub = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);

    /* 윈도우를 생성한다. */
    cvNamedWindow("IMAGE_1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_ADDITION", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_SUBTRACTION", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_MULTIPLICATION", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_DIVISION", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_GRAY1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_GRAY2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("IMAGE_WHITE", CV_WINDOW_AUTOSIZE);

    /* 영상 처리를 진행한다. */
    cvAdd(image1, image2, image_add, NULL); 			/* 영상 합성: 산술 연산(더하기) */
    cvSub(image1, image2, image_sub, NULL); 			/* 영상 합성: 산술 연산(빼기) */
    cvMul(image1, image2, image_mul, 1); 			/* 영상 합성: 산술 연산(곱하기) */
    cvDiv(image1, image2, image_div, 1); 			/* 영상 합성: 산술 연산(나누기) */
    cvCvtColor(image1, image_gray1, CV_RGB2GRAY); 		/* 영상 색상 변환 */
    cvCvtColor(image2, image_gray2, CV_RGB2GRAY);
    cvAbsDiff(image_gray1, image_gray2, image_gray_sub); 	/* 행렬의 차 계산 */
    cvThreshold(image_gray_sub, image_white, 100, 255, CV_THRESH_BINARY); 	/* 임곗값 계산 */

    /* 화면에 처리된 결과를 표시한다. */
    cvShowImage("IMAGE_1", image1);
    cvShowImage("IMAGE_2", image2);
    cvShowImage("IMAGE_ADDITION", image_add);
    cvShowImage("IMAGE_SUBTRACTION", image_sub);
    cvShowImage("IMAGE_MULTIPLICATION", image_mul);
    cvShowImage("IMAGE_DIVISION", image_div);
    cvShowImage("IMAGE_GRAY1", image_gray1);
    cvShowImage("IMAGE_GRAY2", image_gray2);
    cvShowImage("IMAGE_WHITE", image_white);

    /* 키보드가 눌릴 때까지 대기한다. */
    cvWaitKey(0);

    /* 앞에서 사용할 리소스들을 해제한다. */
    cvReleaseImage(&image1);
    cvReleaseImage(&image2);
    cvReleaseImage(&image_add);
    cvReleaseImage(&image_sub);
    cvReleaseImage(&image_mul);
    cvReleaseImage(&image_div);
    cvReleaseImage(&image_gray1);
    cvReleaseImage(&image_gray2);
    cvReleaseImage(&image_white);
    cvReleaseImage(&image_gray_sub);

    /* 모든 윈도우를 삭제한다. */
    cvDestroyAllWindows();

    return 0;
}
