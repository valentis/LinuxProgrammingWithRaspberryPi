#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main(int argc, char **argv)
{
    Mat image1 = imread("sample1.jpg", IMREAD_COLOR);
    Mat image2 = imread("sample2.jpg", IMREAD_COLOR);
    Mat image_add = Mat::zeros(image1.size(), image1.type());
    Mat image_sub = Mat::zeros(image1.size(), image1.type());
    Mat image_mul = Mat::zeros(image1.size(), image1.type());
    Mat image_div = Mat::zeros(image1.size(), image1.type());
    Mat image_gray1 = Mat::zeros(image1.size(), CV_8UC1);
    Mat image_gray2 = Mat::zeros(image1.size(), CV_8UC1);
    Mat image_white = Mat::zeros(image1.size(), CV_8UC1);
    Mat image_gray_sub = Mat::zeros(image1.size(),CV_8UC1);

    /* 영상 처리를 진행한다. */
    add(image1, image2, image_add); 					/* 영상 합성: 산술 연산(더하기) */
    subtract(image1, image2, image_sub); 				/* 영상 합성: 산술 연산(빼기) */
    multiply(image1, image2, image_mul); 				/* 영상 합성: 산술 연산(곱하기) */
    divide(image1, image2, image_div); 					/* 영상 합성: 산술 연산(나누기) */
    cvtColor(image1, image_gray1, CV_BGR2GRAY); 			/* 영상 색상 변환 */
    cvtColor(image2, image_gray2, CV_BGR2GRAY); 			/* 영상 색상 변환 */
    absdiff(image_gray1, image_gray2, image_gray_sub); 			/* 행렬의 차 계산 */
    threshold(image_gray_sub, image_white, 100, 255, THRESH_BINARY); 	/* 임곗값 계산 */

    /* 화면에 처리된 결과를 표시한다. */
    imshow("IMAGE_1", image1);
    imshow("IMAGE_2", image2);
    imshow("IMAGE_ADDITION", image_add);
    imshow("IMAGE_SUBTRACTION", image_sub);
    imshow("IMAGE_MULTIPLICATION", image_mul);
    imshow("IMAGE_DIVISION", image_div);
    imshow("IMAGE_GRAY1", image_gray1);
    imshow("IMAGE_GRAY2", image_gray2);
    imshow("IMAGE_WHITE", image_white);

    /* 키보드가 눌릴 때까지 대기한다. */
    waitKey(0);

    /* 모든 윈도우를 삭제한다. */
    destroyAllWindows();

    return 0;
}
