#ifndef IMAGEPAD_H
#define IMAGEPAD_H

#include <QWidget>
#include <QImage>
#include <QPen>
#include <QTimer>
#include <opencv2/opencv.hpp>

class ImagePad : public QWidget
{
      Q_OBJECT

  public:
      explicit ImagePad(QWidget *parent = nullptr);
      unsigned char* imageData(); 			/* 저수준 이미지 데이터를 반환 */
      const QImage buffer(); 				/* QImage 객체를 반환 */

  protected:
      void mouseMoveEvent(QMouseEvent *ev) final; 	/* 마우스의 커서 이동을 처리 */
      void mousePressEvent(QMouseEvent *ev) final; 	/* 마우스 버튼이 눌러짐을 처리 */
      void mouseReleaseEvent(QMouseEvent *ev) final; 	/* 마우스 버튼이 띄어짐을 처리 */
      void paintEvent(QPaintEvent *ev) final; 		/* 위젯이 그려질 때 자동 호출 */

  public slots:
      void clearWidget(); 				/* 화면을 지움 */
      void capture();
      void startCamera();
      void stopCamera();
      void setFlag(bool flag); 				/* 다음 드로잉때 지울지 설정 */
      void setPixmap(QPixmap pixmap); 			/* 이미지를 설정 */
      
  private:
      QImage Mat2QImage(cv::Mat const& src); /* 이미지 포맷 변환 메소드 */
      cv::Mat QImage2Mat(QImage const& src);

      QImage m_defaultBuffer; 				/* 이미지 데이터를 저장 */
      QPen m_pen; 					/* 펜: 그려질 색과 굵기 등 */
      QPoint m_prevPoint; 				/* 이전 점 */
      QTimer* m_cameraTimer; 				/* 카메라 사용을 위한 타이머 */
      cv::VideoCapture m_capture; 			/* 카메라 사용을 위한 객체 선언 */
      bool m_bStart; 					/* 위젯을 지울 때 설정 */
};

#endif 		/* IMAGEPAD_H */
