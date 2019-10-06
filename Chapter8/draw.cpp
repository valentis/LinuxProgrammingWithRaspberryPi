#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int isDrawing = 0; 				/* 현재 드로잉 중인지를 표시한다. */
Point prevPt; 					/* 선을 그리기 위해 저장될 이전 점의 위치 */
Mat image; 					/* 드로잉될 이미지를 위한 행렬 */

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    Scalar color(0, 0, 0); 			/* 파란색(blue), 초록색(green), 빨간색(red) */
    Point newPt(x, y); 				/* 현재 마우스 커서의 좌표 */

    if(event == EVENT_LBUTTONDOWN) { 		/* 마우스 왼쪽 버튼을 눌렀을 때 */
        isDrawing = 1; 				/* 드로잉 시작 */
        circle(image, newPt, 1, color, -1); 	/* 원 그리기 */
    } else if(event == EVENT_MOUSEMOVE) { 	/* 마우스를 이동했을 때 */
        if(isDrawing) { 			/* 드로잉 중일 때 */
            line(image, prevPt, newPt, color, 1); 	/* 선 그리기 */
        }
    } else if(event == EVENT_LBUTTONUP) { 	/* 마우스 왼쪽 버튼을 띄었을 때 */
        isDrawing = 0; 				/* 드로잉 끝 */
        line(image, prevPt, newPt, color, 1); 	/* 선 그리기 */
    }

    prevPt = newPt; 				/* 다음 드로잉을 위해 현재점을 이전 점에 대입 */
}

int main()
{
    image = Mat(300, 400, CV_8UC3, Scalar(255, 255, 255));
    namedWindow("Draw", WINDOW_NORMAL); 	/* 윈도우의 생성 */

    /* 마우스의 이벤트 처리를 위한 콜백 함수를 등록한다. */
    setMouseCallback("Draw", CallBackFunc, NULL);
    while(1) {
        /* 이미지를 표시한다. */
        imshow("Draw", image); 			/* 이미지를 표시한다. */
        int key = waitKey(1) & 0xFF; 		/* 키보드 입력을 받고 하위 8비트만 취한다. */
        if(key == 27) break; 			/* ESC 키가 눌러지면 루프를 탈출한다. */
    }

    destroyAllWindows(); 			/* 현재 있는 모든 윈도우를 닫는다. */

    return 0;
}
