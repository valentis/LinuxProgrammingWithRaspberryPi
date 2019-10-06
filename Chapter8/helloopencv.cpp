#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
    Mat image(300, 400, CV_8UC1, Scalar(255)); 	/* 흰색 배경의 행렬을 생성한다. */
    imshow("Hello World", image); 		/* "Hello World" 창에 이미지를 표시한다. */
    waitKey(0); 				/* 사용자의 키 입력을 대기한다. */

    return 0;
}
