#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat image = Mat::ones(300, 400, CV_8UC3); 	/* 흰색 배경의 행렬을 생성한다. */
    image.setTo(cv::Scalar(255, 255, 255)); 	/* 흰색으로 채운다. */
    Scalar color(255, 0, 255); 			/* 파란색(blue), 초록색(green), 빨간색(red) */
    Point p1(50, 50), p5(150, 150); 		/* 다각형을 위한 2개의 점을 설정 */

    std::vector<Point> contour;
    contour.push_back(p1);
    contour.push_back(Point(200, 100));
    contour.push_back(Point(250, 50));
    contour.push_back(Point(180, 200));
    contour.push_back(p5);

    const Point *pts = (const cv::Point*) Mat(contour).data;
    int npts = contour.size(); 			/* Mat(contour).rows를 대신 사용할 수 있다. */

    polylines(image, &pts, &npts, 1, true, color);

    imshow("Draw Polygon", image); 		/* "Draw Polygon" 창에 이미지 표시 */
    waitKey(0); 				/* 사용자의 키 입력을 대기한다. */

    return 0;
}

