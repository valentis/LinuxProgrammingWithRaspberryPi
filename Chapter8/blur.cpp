#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat image1 = imread("mandrill.jpg", IMREAD_COLOR);
    Mat image2 = Mat::zeros(image1.size(), image1.type());
    Mat image3 = Mat::zeros(image1.size(), image1.type());

    GaussianBlur(image1, image2, Size(3,3), 3);

    imshow("Image", image1); 		/* 창에 이미지를 표시한다. */
    imshow("GaussianBlur", image2);

    waitKey(0); 			/* 사용자의 키 입력을 대기한다. */

    return 0;
}
