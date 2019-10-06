#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat image1 = imread("mandrill.jpg", IMREAD_COLOR);
    Mat image2 = Mat::zeros(image1.size(), image1.type());
    Mat image3 = Mat::zeros(image1.size(), image1.type());
    Mat image4 = Mat::zeros(image1.size(), image1.type());
    Mat image5 = Mat::zeros(image1.size(), image1.type());
    double angle = 45;
    Point2f center(image1.cols/2., image1.rows/2);

    Mat rotMat = getRotationMatrix2D(center, angle, 1.0);

    warpAffine(image1, image2, rotMat, image1.size());
    flip(image1, image3, 0); 		/* 수직(상하) 대칭 */
    flip(image1, image4, 1); 		/* 수평(좌우) 대칭 */
    transpose(image1, image5);

    imshow("Image", image1); 		/* 창에 이미지 표시 */
    imshow("warpAffine", image2);
    imshow("flip", image3);
    imshow("mirror", image4);
    imshow("transpose", image5);

    waitKey(0); 			/* 사용자의 키 입력을 대기한다. */

    return 0;
}
