#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat image = Mat::zeros(300, 400, CV_8UC3); 	/* 검은색 배경의 이미지를 생성한다. */
    image.setTo(cv::Scalar(255, 255, 255)); 	/* 흰색으로 채운다. */
    Scalar color(255, 255, 0); 			/* 파란색(blue), 초록색(green), 빨간색(red) */
    float scale = 0.8;

    putText(image, "FONT_HERSHEY_SIMPLEX", Point(10, 30),
    FONT_HERSHEY_SIMPLEX, scale, color, 1);
    putText(image, "FONT_HERSHEY_PLAIN", Point(10, 60),
    FONT_HERSHEY_PLAIN, scale, color, 1);
    putText(image, "FONT_HERSHEY_DUPLEX", Point(10, 90),
    FONT_HERSHEY_DUPLEX, scale, color, 1);
    putText(image, "FONT_HERSHEY_COMPLEX", Point(10, 120),
    FONT_HERSHEY_COMPLEX, scale, color, 1);
    putText(image, "FONT_HERSHEY_TRIPLEX", Point(10, 150),
    FONT_HERSHEY_TRIPLEX, scale, color, 1);
    putText(image, "FONT_HERSHEY_COMPLEX_SMALL", Point(10, 180),
    FONT_HERSHEY_COMPLEX_SMALL, scale, color, 1);
    putText(image, "FONT_HERSHEY_SCRIPT_SIMPLEX", Point(10, 210),
    FONT_HERSHEY_SCRIPT_SIMPLEX, scale, color, 1);
    putText(image, "FONT_HERSHEY_SCRIPT_COMPLEX", Point(10, 240),
    FONT_HERSHEY_SCRIPT_COMPLEX, scale, color, 1);
    putText(image, "FONT_HERSHEY_PLAIN + FONT_ITALIC", Point(10, 270),
    FONT_HERSHEY_PLAIN | FONT_ITALIC, scale, color, 1);

    imshow("Draw Polygon", image); 		/* "Draw Polygon" 창에 이미지 표시 */
    waitKey(0); 				/* 사용자의 키 입력을 대기한다. */

    return 0;
}
