#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    Mat image1 = imread("lena.bmp", IMREAD_GRAYSCALE);
    Mat image2 = Mat::zeros(image1.size(), image1.type());
    Mat image3 = Mat::zeros(image1.size(), image1.type());
    Mat image4 = Mat::zeros(image1.size(), image1.type());
    Mat image5 = Mat::zeros(image1.size(), image1.type());
    Mat xEdgeMat, yEdgeMat;
    int ddepth = CV_16S;

    /* 노이즈를 제거하기 위해 가우시안 블러를 수행한다. */
    GaussianBlur(image1, image1, Size(3,3), 0, 0, BORDER_DEFAULT);

    /* 소벨 알고리즘을 적용한다. */
    Sobel(image1, xEdgeMat, ddepth, 1, 0);
    Sobel(image1, yEdgeMat, ddepth, 0, 1);
    convertScaleAbs(xEdgeMat, xEdgeMat);
    convertScaleAbs(yEdgeMat, yEdgeMat);
    addWeighted(xEdgeMat, 0.5, yEdgeMat, 0.5, 0, image2);

    /* 라플라시안 알고리즘을 적용한다. */
    Laplacian(image1, image3, ddepth, 3);
    convertScaleAbs(image3, image3);

    /* 스카 알고리즘을 적용한다. */
    Scharr(image1, xEdgeMat, ddepth, 1, 0);
    Scharr(image1, yEdgeMat, ddepth, 0, 1);
    convertScaleAbs(xEdgeMat, xEdgeMat);
    convertScaleAbs(yEdgeMat, yEdgeMat);
    addWeighted(xEdgeMat, 0.5, yEdgeMat, 0.5, 0, image4);

    /* 캐니 알고리즘을 적용한다. */
    Canny(image1, image5, 50, 150);
    imshow("Image", image1); /* 창에 이미지를 표시한다. */
    imshow("Sobel", image2);
    imshow("Laplacian", image3);
    imshow("Scharr", image4);
    imshow("Canny", image5);

    waitKey(0); /* 사용자의 키 입력을 대기한다. */

    return 0;
}
