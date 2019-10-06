#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
    Mat image = imread("mandrill.jpg", IMREAD_COLOR);

    image += 50;

    imshow("Mat : Plus", image); 	/* "Mat : Plus" 창에 이미지를 표시한다. */

    waitKey(0); 			/* 사용자의 키 입력을 대기한다. */

    return 0;
}
