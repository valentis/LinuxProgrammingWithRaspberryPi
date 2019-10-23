#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <opencv2/opencv.hpp> 		/* OpenCV를 위한 헤더 파일 */

#include "classification.h"

class QTextBrowser;
class QLabel;
class QTimer;

class Widget : public QWidget
{
    Q_OBJECT
 public:
      Widget(QWidget *parent = nullptr);
      ~Widget();

  protected:
      /* OpenCV와 Qt의 이미지 포맷 변경을 위한 메소드 */
      QImage Mat2QImage(cv::Mat const& src);
      cv::Mat QImage2Mat(QImage const& src);

  private slots:
      void checkImage(); 		/* 이미지를 분류한다. */
      void camera(bool); 		/* 카메라를 시작하거나 끈다. */
      void startCamera(); 		/* 카메라를 시작한다. */
      void stopCamera(); 		/* 카메라를 끈다. */
      void capture(); 			/* 이미지를 캡쳐한다. */
      void openFile(); 			/* 이미지 파일을 불러온다. */

  private:
      Classifier m_classifier; 		/* 이미지 분류를 위한 Classifier 클래스의 객체 */
      QLabel* m_imageDisp; 		/* 이미지 출력을 위한 QLabel 위젯 */
      QTextBrowser* m_resultDisp; 	/* 이미지 분류 결과 출력을 위한 QTextBrowser 위젯 */
      QTimer* m_cameraTimer; 		/* 카메라 동작을 위한 타이머 */
      cv::VideoCapture m_capture; 	/* 카메라 사용을 위한 OpenCV의 VideoCapture 객체 */
      cv::Mat m_defaultBuffer; 		/* 이미지 데이터를 저장을 위한 버퍼 */
};

#endif /* WIDGET_H */
