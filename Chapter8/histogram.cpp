#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
    MatND hist;
    const int* chan_nos = { 0 };
    float chan_range[] = { 0.0, 255.0 };
    const float* chan_ranges = chan_range;
    int histSize = 255;
    double maxVal = 0, minVal = 0; /* 최대, 최소 빈도수 */

    Mat image1 = imread("mandrill.jpg", IMREAD_GRAYSCALE);
    Mat image2 = Mat::zeros(image1.size(), image1.type());
    Mat histImg1(histSize, histSize, CV_8U, Scalar(histSize)); 		/* 히스토그램 출력 행렬 */
    Mat histImg2(histSize, histSize, CV_8U, Scalar(histSize)); 		/* 히스토그램 출력 행렬 */

    image2 = image1 + 50;

    calcHist(&image1, 1, 			/* 단일 영상의 히스토그램만 */
             chan_nos, 				/* 대상 채널 */
             Mat(), 				/* 마스크 사용하지 않음 */
             hist, 				/* 결과 히스토그램 */
             1, 				/* 1차원(1D) 히스토그램 */
             &histSize, 			/* 빈도 수 */
             &chan_ranges); 			/* 화소값 범위 */

    minMaxLoc(hist, &minVal, &maxVal, 0, 0);
    int hpt = static_cast<int>(0.9 * histSize); 	/* 90%를 최대점으로 설정 */
    for(int h = 0; h < histSize; h++) { 		/* 각 빈도에 대한 수직선을 그리기 */
        float binVal = hist.at<float>(h);
        int intensity = static_cast<int>(binVal * hpt / maxVal);

        /* 두 점 간의 거리를 그리는 함수 */
        line(histImg1, Point(h, histSize), Point(h, histSize - intensity), Scalar::all(0));
    }

    calcHist(&image2, 1, 			/* 단일 영상의 히스토그램만 */
             chan_nos, 				/* 대상 채널 */
             Mat(), 				/* 마스크 사용하지 않음 */
             hist, 				/* 결과 히스토그램 */
             1, 				/* 1차원(1D) 히스토그램 */
             &histSize, 			/* 빈도수 */
             &chan_ranges); 			/* 화소값 범위 */

    minMaxLoc(hist, &minVal, &maxVal, 0, 0);
    for(int h = 0; h < histSize; h++) { 	/* 각 빈도에 대한 수직선 그리기 */
        float binVal = hist.at<float>(h);
        int intensity = static_cast<int>(binVal * hpt / maxVal);
        line(histImg2, Point(h, histSize), Point(h, histSize - intensity), Scalar::all(0));
    }

    imshow("Histogram1", histImg1); 		/* 창에 히스토그램 분포 이미지 표시 */
    imshow("Histogram2", histImg2);

    waitKey(0); 				/* 사용자의 키 입력을 대기한다. */

    return 0;
}
