#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat image = Mat::zeros(300, 400, CV_8UC3); 		/* 흰색 배경의 행렬을 생성한다. */
    image.setTo(cv::Scalar(255, 255, 255)); 		/* 흰색으로 채운다. */
    Scalar color(0, 0, 255); 				/* 파란색(blue), 초록색(green), 빨간색(red) */
    Point p1(100, 100), p2(220, 100); 			/* 타원을 위한 2개의 점을 설정 */
    Size size(50, 40);

    circle(image, p1, 50, color, -1); 			/* 원 그리기 */
    ellipse(image, p2, size, 45, 0, 270, color, -1); 	/* 타원 그리기 */

    imshow("Draw Circle", image); 			/* "Draw Circle" 창에 이미지 표시 */
    waitKey(0); 					/* 사용자의 키 입력을 대기한다. */

    return 0;
}
